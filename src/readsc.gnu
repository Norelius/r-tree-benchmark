set key left top vertical
#Right noreverse noenhanced autotitle nobox
set style data linespoints
set title "Reads for RQ benchmarks scaled with size." 
set xlabel "Thousand of elements"
set ylabel "Reads per RQ"

set xtics 0 850


set term postscript eps enhanced color "Helvetica" 18
set output "readsc.ps"
plot 'rstar100.dat' using ($2/1000.0):4 title 'R*-tree', \
	 'quadratic100.dat' using ($2/1000.0):4 title 'Quadtree' lc rgb 'blue'
set term x11