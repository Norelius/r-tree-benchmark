set key left top vertical
#Right noreverse noenhanced autotitle nobox
set style data linespoints
set title "Execution time for RQ benchmarks scaled with density." 
set xlabel "Thousand of elements per 500 {/Symbol m}m^{3}"
set ylabel "RQ execution time in ms"

set xtics 0 850


set term postscript eps enhanced color "Helvetica" 18
set output "time.ps"
plot 'rstar100.dat' using ($2/1000.0):3 title 'R*-tree', \
	 'quadratic100.dat' using ($2/1000.0):3 title 'Quadtree' lc rgb 'blue'
set term x11