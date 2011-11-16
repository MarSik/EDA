# author: Amand Tihon
# email: amand.tihon@alrj.org
# dist-license: GPL3, http://www.gnu.org/licenses/gpl-3.0.txt
# use-license: unlimited


Element["" "" "C000" "" 20000 10000 -3000 7000 0 100 ""]
(
	Pin[-10000 0 8000 2000 8600 1500 "1" "1" "edge2"]
	Pin[10000 0 8000 2000 8600 1500 "2" "2" "edge2"]
	ElementLine [-2000 0 -10000 0 1500]
	ElementLine [10000 0 2000 0 1500]
	ElementLine [-2000 -5000 -2000 5000 1500]
	ElementLine [2000 -5000 2000 5000 1500]

	)
