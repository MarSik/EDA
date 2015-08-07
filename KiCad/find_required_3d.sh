#!/bin/sh
function find_models() {
  find . -name '*.kicad_mod' -type f -exec cat '{}' \; | grep '(model .*\.wrl' | sed -e 's/(model \(.*\.wrl\)/\1/' | sort -u
}

NEW=0
OK=0
LOCAL=0
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
        OK=$(expr $OK + 1)
        echo -n " .. OK"
      else
        NEW=$(expr $NEW + 1)
        echo -n " .. NEW"
      fi
      cp "$SOURCE/$line" $DEST/$line
    else
      if [ -r $DEST/$line ]; then
        echo -n " .. LOCAL"
        LOCAL=$(expr $LOCAL + 1)
      else
        echo -n " .. NOTFOUND"
        NOTFOUND=$(expr $NOTFOUND + 1)
      fi
    fi
  fi
  echo
done

echo
echo "Imported models: $NEW"
echo "Updated models: $OK"
echo "Preserved models: $LOCAL"
echo "Missing models: $NOTFOUND"

