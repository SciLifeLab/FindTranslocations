import argparse
import os
import TIDDIT_clustering

from sklearn.cluster import DBSCAN
from sklearn import metrics
from sklearn.datasets.samples_generator import make_blobs
from sklearn.preprocessing import StandardScaler
from scipy.stats import norm

version = "2.0.0"
parser = argparse.ArgumentParser("""TIDDIT-{}""".format(version),add_help=False)
parser.add_argument('--sv'       , help="call structural variation", required=False, action="store_true")
parser.add_argument('--cov'        , help="generate a coverage bed file", required=False, action="store_true")
args, unknown = parser.parse_known_args()

if args.sv:
	parser = argparse.ArgumentParser("""TIDDIT --sv -b inputfile [-o prefix] --ref ref.fasta""")
	parser.add_argument('--sv'       , help="call structural variation", required=False, action="store_true")
	parser.add_argument('-b', type=str,required=True, help="coordinate sorted bam file(required)")
	parser.add_argument('-o', type=str,default="output", help="output prefix(default=output)")
	parser.add_argument('-i', type=int, help="paired reads maximum allowed insert size. Pairs aligning on the same chr at a distance higher than this are considered candidates for SV (default=3std + mean_insert_size)")
	parser.add_argument('-d', type=str,help="expected reads orientations, possible values \"innie\" (-> <-) or \"outtie\" (<- ->). Default: major orientation within the dataset")
	parser.add_argument('-p', type=int,default=5, help="Minimum number of supporting pairs in order to call a variation event (default 5)")
	parser.add_argument('-r', type=int,default=5, help="Minimum number of supporting split reads to call a small variant (default 5)")
	parser.add_argument('-q', type=int,default=10, help="Minimum mapping quality to consider an alignment (default 10)")
	parser.add_argument('-Q', type=int,default=10, help="Minimum regional mapping quality (default 20)")
	parser.add_argument('-n', type=int,default=2, help="the number of sets of chromosomes,(default = 2)")
	parser.add_argument('-m', type=int,default=100, help="minimum variant size,(default = 100)")
	parser.add_argument('-e', type=int, help="clustering distance  parameter, discordant pairs closer than this distance are considered to belong to the same variant(default = sqrt(insert-size*2)*12)")
	parser.add_argument('-l', type=int,default=3, help="min-pts parameter (default=3),must be set > 1")
	parser.add_argument('-z', type=int,default=100, help="minimum variant size (default=100)")
	parser.add_argument('-s',default="1,15,14,9,2", type=str, help="a list of chromosomes used to compute the pl,oidy across the chromosomes of the genome(based on coverage), these are asseumed to have -n ploidy default (1,5,14,9,2)")
	parser.add_argument('--ref',required=True, type=str, help="reference fasta")
	#parser.add_argument('-v', type=float,default=0.5, help="overlap_limit, signals overlapping less than this value are removed from the cluster")

	args= parser.parse_args()
	args.wd=os.path.dirname(os.path.realpath(__file__))
	if args.l < 1:
		print "error, too low --l value!"
		quit()

	command_str="{}/bin/TIDDIT --sv -b {} -o {} -p {} -r {} -q {} -n {} -m {}".format(args.wd,args.b,args.o,args.p,args.r,args.q,args.n,args.m)
	if args.i:
		command_str += " -i {}".format(args.i)
	if args.d:
		command_str += " -d {}".format(args.d)

	#os.system(command_str)
	TIDDIT_clustering.cluster(args)

elif args.cov:
	parser = argparse.ArgumentParser("""TIDDIT --cov -b inputfile [-o prefix]""")
	parser.add_argument('--cov'        , help="generate a coverage bed file", required=False, action="store_true")
	parser.add_argument('-b', type=str,required=True, help="coordinate sorted bam file(required)")
	parser.add_argument('-o', type=str,default="output", help="output prefix(default=output)")
	parser.add_argument('-z', type=int,default=500, help="use bins of specified size(default = 500bp) to measure the coverage of the entire bam file, set output to stdout to print to stdout")
	args= parser.parse_args()
	args.wd=os.path.dirname(os.path.realpath(__file__))

	os.system("{}/bin/TIDDIT --cov -b {} -o {} -z {}".format(args.wd,args.b,args.o,args.z))
else:
	parser.print_help()
