DESCRIPTION
==============
TIDDIT: Is a tool to used to identify  chromosomal rearrangements using Mate Pair or Paired End sequencing data. TIDDIT identifies intra and inter-chromosomal translocations, deletions, tandem-duplications and inversions, using supplementary alignments as well as discordant pairs.

TIDDIT has two modes of analysing bam files. The sv mode, which is used to search for structural variants. And the cov mode that analyse the read depth of a bam file and generates a coverage report.


INSTALLATION
==============
TIDDIT requires standard c++/c libraries, python 2.7, and Numpy. To compile TIDDIT, cmake must be installed. 

```
git clone https://github.com/SciLifeLab/TIDDIT.git
```

To install TIDDIT:
```
cd TIDDIT
./INSTALL.sh
```

TIDDIT is run via the TIDDIT.py script:
```

python TIDDIT.py --help
python TIDDIT.py  --sv --help
python TIDDIT.py  --cov --help
```

TIDDIT is also distributed with a Singularity environment (http://singularity.lbl.gov/index.html). Type the following command to download the container:

    singularity pull --name TIDDIT.simg shub://J35P312/TIDDIT

Type the following to run tiddit:

    singularity exec TIDDIT.simg TIDDIT.py

You may also build it yourself (if you have sudo permisions)

    singularity build TIDDIT.simg Singularity

The singularity container will download and install the latest commit on the scilifelab branch of TIDDIT.

The SV module
=============
The main TIDDIT module, detects structural variant using discordant pairs, split reads and coverage information

    python TIDDIT.py --sv [Options] --bam bam --ref reference.fasta

NOTE: It is important that you use the TIDDIT.py wrapper for SV detection. The TIDDIT binary in the TIDDIT/bin folder does not perform any clustering, it simply extract SV signatures into a tab file.

Where bam is the input bam file. And reference.fasta is the reference fasta used to align the sequencing data: TIDDIT will crash if the reference fasta is different from the one used to align the reads. The reads of the input bam file must be sorted on genome position.
TIDDIT may be fine tuned by altering these optional parameters:

    -o - The prefix of the output files(default = output)
        
    -i - The maximum allowed insert size of a normal pair. Pairs having larger insert 
         than this is treated as discordant pairs. Default is 3*std+mean insert size
                        
    -d - The pair orientation, use this setting to override the automatic orientation selection

    -l - The density parameter, to create a cluster, more than l signals (split reads+ discordant pairs) must be present, signals are added to a cluster if they are neighbouring atleast this  number of signals (defualt 4, minimum 2)
            
    -p - The minimum number of discordant pairs and supplementary alignments used to call large SV. Default is 4
    
    -r - The minimum number of supplementary alignments used to call small SV. Default is 4
            
    -q - The minimum mapping quality of the discordant pairs/supplementary alignments 
         forming a variant. Default value is 10.

    -n - The ploidy of the organism ,(default = 2)

    --force_ploidy - set the ploidy of all chromosomes to -n (including the sex chromosomes), this option will disable the ploidy estimation.
                     This option is meant to be used for low quality data or for species having equal ploidy across all chromosomes

output:

TIDDIT SV module produces three output files, a vcf file containing SV calls, a tab file describing the coverage across the genome in bins of size 100 bp, and a tab file dscribing the estimated ploidy and coverage across each contig.

Useful settings:

It may be useful to increase the precision of TIDDIT, especially when searching the entire genome for disease causing variants.
on 30X bam files, I usually set -p 7 and -r 5.

On low coverage matepair data, it may be usedul to lower the -l parameter to 2.

In noisy datasets you may get too many small variants. If this is the case, then you may increase the -l parameter, or set the -i parameter to a high value (such as 2000).
                                        
The cov module
==============
Computes the coverge of different regions of the bam file

    python TIDDIT.py --cov [Options] --bam bam
    
optional parameters:

    -o - the prefix of the output files
    -z - compute the coverage within bins of a specified size across the entire genome, default bin size is 500

Filters
=============
TIDDIT uses four different filters to detect low quality calls. The filter field of variants passing these tests are set to "PASS". If a variant fail any of these tests, the filter field is set to the filter failing that variant. These are the four filters empoyed by TIDDIT:

    Expectedlinks
        The number of discordant pairs/supplementary alignments supporting
        the variant is less than 60% of the expected number of supporting reads
    FewLinks
        The number of discordant pairs supporting the variant is too low compared to the number of discordant pairs within that genomic region.
    Unexpectedcoverage
        High coverage
    Smear
        The two windows that define the regions next to the breakpoints overlap.

Failed Variants may be removed using tools such as VCFtools or grep. Removing these variants greatly improves the precision of TIDDIT, but may reduce the sensitivity. It is adviced to remove filtered variants or prioritize the variants that have passed the quality checks.
This command may be usedto filter the TIDDIT vcf:

	grep -E "#|PASS" input.vcf > output.filtered.vcf

Contents of the VCF INFO field
=============
The INFO field of the VCF contains the following entries:

    SVTYPE
        Type of structural variant(DEL,DUP,BND,INV,TDUP)
    END
        End position of an intra-chromosomal variant
    LFA
        The number of discordant pairs at the the first breakpoint of the variant
    LFB
	The number of discordant pairs at the the second breakpoint of the variant
    LTE
        The number of discordnat pairs that form the structural variant.
    COVA
        Coverage on window A
    COVM
        The coverage between A and B
    COVB
        Coverage on window B
    CIPOS
        start and stop positon of window A
    CIEND
        start and stop position of window B
    QUALA
        The average mapping quality of the reads in window A
    QUALB
        The average mapping quality of the reads in window B
    E1
        Expected number of discordant pairs - assuming normal distribution (similar to the model used by the BESST scaffolder)
    E2
        Expected number of discordant pairs - assuming uniform coverage

The content of the INFO field can be used to filter out false positives and to gain more understanding of the structure of the variant. More info is found in the vcf file. 

Algorithm
=========

Discordant pairs and split reads (supplementary alignments) are extracted and stored in the ".signals.tab" file. A discordant pair is any pair having a larger insert size than the  -i paramater, or a pair where the reads map to different chromosomes.
supplementary alignments and discordant pairs are only extracted if their mapping quality exceed the -q parameter.

The most recent version of TIDDIT uses an algorithm similar to DBSCAN: A cluster is formed if -l or more signals are located within the -e distance. Once a cluster is formed, more signals may be added if these signals are within the
-e distance of -l signals within a cluster.

A cluster is rejected if it contains less than -r plus -p signals. If the cluster is rejected, it will not be printed to the vcf file.

If the cluster is not rejected, it will be printed to file, even if it fails any quality filter. 

The sensitivity and precision may be controlled using the -q,r,p, and -l parameters. 

LICENSE
==============
All the tools distributed with this package are distributed under GNU General Public License version 3.0 (GPLv3). 



