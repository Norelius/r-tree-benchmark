set key left top vertical
#Right noreverse noenhanced autotitle nobox
set style data linespoints
set title "Reads for RQ benchmarks." 
set xlabel "Thousand of elements per 500 {/Symbol m}m^{3}"
set ylabel "Reads per RQ"

set xtics 0 850


set term postscript eps enhanced color "Helvetica" 18
set output "reads.ps"
plot 'rstar.dat' using ($2/1000.0):4 title 'R*'
set term x11