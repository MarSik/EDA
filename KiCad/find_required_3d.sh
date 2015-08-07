#!/bin/sh
function find_models() {
  find . -name '*.kicad_mod' -type f -exec cat '{}' \; | grep '(model .*\.wrl' | sed -e 's/(model \(.*\.wrl\)/\1/' | sort -u
}

NEW=0
UPDATED=0
OK=0
NOTFOUND=0

DEST=packages3d

for line in $(find_models); do
  echo -n $line
  if [ $# -gt 0 ]; then
    SOURCE=$1

    if [ -r "$SOURCE/$line" ]; then
      DIR=$(dirname $line)
      mkdir -p $DEST/$DIR
      if [ -r $DEST/$line ]; then
        UPDATED=$(expr $UPDATED + 1)
        echo -n " .. UPDATED"
      else
        NEW=$(expr $NEW + 1)
        echo -n " .. NEW"
      fi
      cp "$SOURCE/$line" $DEST/$line
    fi
  fi

  if [ -r $DEST/$line ]; then
    echo -n " .. OK"
    OK=$(expr $OK + 1)
  else
    echo -n " .. NOTFOUND"
    NOTFOUND=$(expr $NOTFOUND + 1)
  fi

  echo
done

echo
echo "Imported models: $NEW"
echo "Updated models: $UPDATED"
echo "Existing models: $OK"
echo "Missing models: $NOTFOUND"

