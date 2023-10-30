#!/bin/bash


cd Lib/SamacSys_Parts.pretty/
find ./ -name "*.kicad_mod" -exec sed -i .bk 's@C:\\\\Users\\\\Marco\\\\Documents\\\\KiCad\\\\7.0\\\\SamacSys_Parts.3dshapes\\\\@${KIPRJMOD}/Lib/SamacSys_Parts.3dshapes/@g' '{}' \;