PCBNEW-LibModule-V1  Thu 29 Sep 2011 04:23:47 PM EDT
# encoding utf-8
$INDEX
SMA_EDGE
VIA
$EndINDEX
$MODULE VIA
Po 0 0 0 15 4E81F156 00000000 ~~
Li VIA
Sc 00000000
AR
Op 0 0 0
T0 0 -500 600 600 0 120 N V 21 N ""
T1 0 500 600 600 0 120 N V 21 N ""
$PAD
Sh "o" C 394 394 0 0 0
Dr 236 0 0
At STD N 00E0FFFF
Ne 0 ""
Po 0 0
$EndPAD
$EndMODULE  VIA
$MODULE SMA_EDGE
Po 0 0 0 15 4E84D3BE 00000000 ~~
Li SMA_EDGE
Sc 00000000
AR /4E7D5DB2
Op 0 0 0
T0 0 0 600 600 0 120 N V 21 N "SMA_E"
T1 0 1000 600 600 0 120 N I 21 N "SMA_EDGE"
DS -2500 -1010 -2500 1990 150 21
DS -2500 1990 2500 1990 150 21
DS 2500 1990 2500 -1010 150 21
DS 2500 -1010 -2500 -1010 150 21
$PAD
Sh "0" R 1800 600 0 0 0
Dr 0 0 0
At CONN N 0080FFFF
Ne 2 "N-000008"
Po -3495 -746
$EndPAD
$PAD
Sh "1" T 197 551 236 0 0
Dr 0 0 0
At CONN N 00800001
Ne 1 "N-000001"
Po -2700 500
$EndPAD
$PAD
Sh "2" R 1800 600 0 0 0
Dr 0 0 0
At CONN N 0080FFFF
Ne 2 "N-000008"
Po -3495 1744
$EndPAD
$PAD
Sh "1" R 1603 787 0 0 0
Dr 0 0 0
At CONN N 00800001
Ne 0 ""
Po -3600 500
$EndPAD
$SHAPE3D
Na "SMA_EDGE.wrl"
Sc 1.300000 1.300000 1.300000
Of -0.205000 -0.050000 -0.120000
Ro 270.000000 0.000000 180.000000
$EndSHAPE3D
$EndMODULE  SMA_EDGE
$EndLIBRARY