set key left top vertical
#Right noreverse noenhanced autotitle nobox
set style data linespoints
set title "Execution time for RQ benchmarks scaled with size." 
set xlabel "Thousand of elements"
set ylabel "RQ execution time in ms"

set xtics 0 850


set term postscript eps enhanced color "Helvetica" 18
set output "timec9.ps"
plot 'rstar100constant.dat' using ($2/1000.0):3 title 'R*-tree', \
	 'quadratic100constant.dat' using ($2/1000.0):3 title 'Quadtree' lc rgb 'blue'
set term x11