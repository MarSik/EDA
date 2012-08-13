# author: Amand Tihon
# email: amand.tihon@alrj.org
# dist-license: GPL3, http://www.gnu.org/licenses/gpl-3.0.txt
# use-license: unlimited


Element["" "" "C000" "" 30000 10000 -2500 7500 0 100 ""]
(
	Pin[-20000 0 8000 2000 8600 1500 "1" "1" "edge2"]
	Pin[20000 0 8000 2000 8600 1500 "2" "2" "edge2"]
	ElementLine [2000 0 20000 0 1500]
	ElementLine [-2000 -5000 -2000 5000 1500]
	ElementLine [2000 -5000 2000 5000 1500]
	ElementLine [-20000 0 -2000 0 1500]

	)
