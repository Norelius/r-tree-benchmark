set key left top vertical
#Right noreverse noenhanced autotitle nobox
set style data linespoints
set title "Build time scaled with size."
set xlabel "Thousand of elements"
set ylabel "Build time in seconds"

set xtics 0 850


set term postscript eps enhanced color "Helvetica" 18
set output "buildc.ps"
plot 'rstar100constant_build.dat' using ($2/1000.0):3 title 'R*-tree', \
	 'quadratic100constant_build.dat' using ($2/1000.0):3 title 'Quadtree' lc rgb 'blue'
set term x11