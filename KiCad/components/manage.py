#!/bin/python
import sys
import os

def explode(name, suffix=".lib", start="DEF ", stop="ENDDEF"):
    dirname = name + ".library"
    try:
        os.makedirs(dirname)
    except OSError:
        pass

    with open(name+suffix, "r") as fin:
        header = fin.readline()
        state = 0 # nothing
        fout = None
        encoding = None
        for line in fin:
            if state == 0 and line.startswith(start):
                parts = line.split()
                name = parts[1]
                if name[0] == "~":
                    name = name[1:]
                fout = open(os.path.join(dirname, name + suffix), "w")
                fout.write(header)
                if encoding:
                    fout.write(encoding)
                fout.write(line)
                state = 1
            elif state == 1 and line.startswith(stop):
                fout.write(line)
                fout.close()
                state = 0
            elif state == 1:
                fout.write(line)
            elif state == 0 and line.startswith("#encoding"):
                encoding = line

def compile(dirname):
    libname, ext = dirname.split(".")
    if ext != "library":
        print "This does not seem to be a library dir"
        return

    starts = {
        "lib": "DEF ",
        "dcm": "$CMP "
    }

    ends = {
        "lib": "ENDDEF",
        "dcm": "$ENDCMP"
    }

    found = set()

    for fname in os.listdir(dirname):
        if fname == "." or fname == "..":
            continue

        name, ext = fname.split(".")
        libfile = libname + "." + ext
        with open(libfile, "a") as dest, open(os.path.join(dirname, fname)) as src:
            if not ext in found:
                dest.truncate(0)
                found.add(ext)

            else:
                # Write separator
                dest.write("#\n# %s\n#\n" % name)

                # Skip header
                if ext in starts:
                    while True:
                        l = src.readline()
                        if l.startswith(starts[ext]):
                            dest.write(l)
                            break


            # Copy the file contents except the tail
            # The file can possibly contain multiple elements..
            state = 1
            for l in src:
                if state == 1:
                    dest.write(l)

                if state == 1 and l.startswith(ends[ext]):
                    state = 0
                elif state == 0 and l.startswith(starts[ext]):
                    state = 1
                    dest.write(l)


if __name__ == "__main__":
    if len(sys.argv) == 1:
        cmd = None
        lib = "."
    else:
        cmd = sys.argv[1]
        lib = sys.argv[2]

    if cmd == "explode":
        explode(lib, suffix=".lib", start="DEF ", stop="ENDDEF")
        explode(lib, suffix=".dcm", start="$CMP ", stop="$ENDCMP")
    elif cmd == "compile":
        compile(lib)
    else: # compile all
        for f in os.listdir(lib):
            if f.endswith(".library"):
                compile(f)

