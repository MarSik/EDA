#PROJECT = 
#SCHEMAS = 
PACKAGES = ~/Dropbox/EDA/repo/pcb-parts


all: drc pcb
drc: $(PROJECT).drc

pcb: $(SCHEMAS) $(PROJECT).drc
	gsch2pcb -d $(PACKAGES) -o $(PROJECT) --gnetlist partslist3 $(SCHEMAS)

$(PROJECT).drc: $(SCHEMAS)
	gnetlist -g drc2 -o $(PROJECT).drc $(SCHEMAS)
	cat $(PROJECT).drc
	grep -q ERROR $(PROJECT).drc && exit 1

clean:
	rm -f *.raw *.drc *.partslist* *.net

dist-clean: clean
	rm -f *.cir *.pcb

.sch.cir:
	gnetlist -g spice-sdb -o $@ $<

.cir.raw:
	ngspice -b -r $@ $<

.SUFFIXES: .sch .cir .raw .net
.PRECIOUS: .drc .sch .partslist3 .pcb .new.pcb .net .cir .raw

