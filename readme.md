THIS IS THE README FILE for efficient matrix multiplication program
Author: G.P.Kanchana 

Please note the program has been implemented using g++ (linux)

#################### How to Run ########################
STEP1.
	Extract the zip archive.then from terminal move to the directory.

STEP2.
	To build and run the program,run the shell script run.sh by the following command
	"./run.sh"
#################### END OF HOW TO RUN ##################


################# Sample Count explanation ##############
For each n ,the execution time taken by serial version,parallel version and optimized version .Then the speedup
of the parallel version and optimized version is calculated and the difference is calculated.
For each test case,initial 10 round of samples will be calculated and mean and standard deviation is taken.The minimum
required iteration count is calculated using the mean and standard deviation with z=1.96 and r=5 for each n each version.if the minimum
required iteration count >10 ,the test case will be rerun minimum_count+1 times and the new mean is taken as the execution time.
############## End of Sample count explanation ##########

#################### Output explanation #################
<n>
serial:<time taken by serial version>
parallel:<time taken by parallel version>
optimized:<time taken by optimized version>
parallel_speedup:<parallel speedup>	optimized_speedup:<optimized speedup>   Diff:<speedup difference>

#################### End of Output explanation ##########


#################### Debug info #########################
If debug info is required please enable -g flag in build command without other optimizations.
#################### End Debug Info #####################
