../build/bin/MoRIS_run \
-map ./input/map.txt \
-network ./input/network.txt \
-sample ./input/sample.txt \
-typeofdata PRESENCE_ABSENCE \
-seed 123 \
-reps 1000 \
-iters 25 \
-law LOG_NORMAL \
-optimfunc LOG_LIKELIHOOD \
-humanactivity YES \
-wmin 0.0 \
-xintro 59891.22 \
-yintro 31742.16 \
-pintro 1.0 \
-lambda 23.13390 \
-mu 6.4951562 \
-sigma 0.174736073 \
-gamma 0.0 \
-w1 0.8232030 \
-w2 0.10088199 \
-w3 0.073098341 \
-w4 0.013589526 \
-w5 0.0 \
-w6 0.0 \
-save-outputs

Rscript ./rscripts/plot_single_simulation.R $(pwd)
