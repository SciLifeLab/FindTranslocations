/*
 * Translocations.cpp
 *
 *  Created on: Jul 10, 2013
 *      Author: vezzi, Eisfeldt
 */

#include "Translocation.h"
#include <string>
#include <cmath>  

string int2str(int to_be_converted){
	string converted= static_cast<ostringstream*>( &(ostringstream() << to_be_converted) )->str();
	return(converted);
}

//this function tries to classify intrachromosomal events
vector<string> Window::classification(int chr, int startA,int endA,double covA,int startB,int endB,double covB,int meanInsert,int STDInsert,bool outtie,float covAB){
	vector<string> svVector;
	return(svVector);
}

string filterFunction(double RATIO,int links, int links_spurr, float mean_insert, float std_insert,double coverageA,double coverageB,double coverageAVG,int endA, int startB, int chrA, int chrB,int ploidy){
	string filter = "PASS";
	return(filter);
}

bool sortMate(long i, long  j) {
	return (i < j);
}

string Window::VCFHeader(string libraryData){
	string headerString ="";
	//Define fileformat and source
	headerString+="##fileformat=VCFv4.1\n";
	headerString+="##source=TIDDIT-" + this-> version +  "\n";
	//define the alowed events
	headerString+="##ALT=<ID=DEL,Description=\"Deletion\">\n";
	headerString+="##ALT=<ID=DUP,Description=\"Duplication\">\n";
	headerString+="##ALT=<ID=TDUP,Description=\"Tandem duplication\">\n";
	headerString+="##ALT=<ID=INV,Description=\"Inversion\">\n";
	headerString+="##ALT=<ID=INS,Description=\"Insertion\">\n";
	headerString+="##ALT=<ID=BND,Description=\"Break end\">\n";

	for(int i=0;i < this -> contig_ids.size(); i++){
		headerString+= "##contig=<ID="	+ contig_ids[i] + ",length=" + contig_length[i];
		if(contig_ids.size() == contig_assembly.size()){
			headerString+=",assembly=" +contig_assembly[i];
		}
		headerString +=">\n";
	}
	
	//Define the info fields
	headerString+="##INFO=<ID=SVTYPE,Number=1,Type=String,Description=\"Type of structural variant\">\n";
	headerString+="##INFO=<ID=END,Number=1,Type=String,Description=\"End of an intra-chromosomal variant\">\n";
	headerString+="##INFO=<ID=SVLEN,Number=.,Type=Integer,Description=\"Difference in length between REF and ALT alleles\">\n";
	headerString+="##INFO=<ID=LFA,Number=1,Type=Integer,Description=\"Links from window A\">\n";
	headerString+="##INFO=<ID=LFB,Number=1,Type=Integer,Description=\"Links from window B\">\n";
	headerString+="##INFO=<ID=LTE,Number=1,Type=Integer,Description=\"Links to event\">\n";
	headerString+="##INFO=<ID=COVA,Number=1,Type=Float,Description=\"Coverage on window A\">\n";
	headerString+="##INFO=<ID=COVM,Number=1,Type=Float,Description=\"The coverage between A and B\">\n";
	headerString+="##INFO=<ID=COVB,Number=1,Type=Float,Description=\"Coverage on window B\">\n";
	headerString+="##INFO=<ID=OR,Number=4,Type=Integer,Description=\"Orientation of the pairs (FF,FR,RF,FR)\">\n";
	headerString+="##INFO=<ID=ORSR,Number=2,Type=Integer,Description=\"Orientation of the split reads (inverted,normal)\">\n";
	headerString+="##INFO=<ID=CIPOS,Number=2,Type=Integer,Description=\"Confidence interval around POS for imprecise variants\">\n";
	headerString+="##INFO=<ID=CIEND,Number=2,Type=Integer,Description=\"Confidence interval around END for imprecise variants\">\n";
	headerString+="##INFO=<ID=EL,Number=1,Type=Float,Description=\"Expected links to window B\">\n";
	headerString+="##INFO=<ID=EL2,Number=1,Type=Float,Description=\"Expected links to window B\">\n";
	headerString+="##INFO=<ID=QUALA,Number=1,Type=Float,Description=\"The average mapping quality of the reads in window A\">\n";
	headerString+="##INFO=<ID=QUALB,Number=1,Type=Float,Description=\"The average mapping quality of the reads in window B\">\n";
	//set filters
	headerString+="##FILTER=<ID=BelowExpectedLinks,Description=\"The number of links or reads between A and B is less than 40\% of the expected value\">\n";
	headerString+="##FILTER=<ID=FewLinks,Description=\"Unexpectedly low fraction of discordant reads betwen A and B\">\n";
	headerString+="##FILTER=<ID=UnexpectedCoverage,Description=\"The coverage of the window on chromosome B or A is higher than 4*average coverage\">\n";
	headerString+="##FILTER=<ID=Smear,Description=\"window A and Window B overlap\">\n";
	headerString+="##FILTER=<ID=RegionalQ,Description=\"The mapping quality of the region is lower than the user set limit\">\n";
	headerString+="##FILTER=<ID=MinSize,Description=\"The variant is smaller than the user set limit\">\n";
	headerString+="##FILTER=<ID=Ploidy,Description=\"Intrachromosomal variant on a chromosome having 0 ploidy\">\n";
	headerString+="##FILTER=<ID=SplitsVSDiscs,Description=\"large variant supported mainly by split reads (and not discorant pairs) \">\n";
	headerString+="##FILTER=<ID=Density,Description=\"The discordant reads cluster too tighlt\">\n";
	//set format 
	headerString+="##FORMAT=<ID=GT,Number=1,Type=String,Description=\"Genotype\">\n";
	headerString+="##FORMAT=<ID=CN,Number=1,Type=Integer,Description=\"Copy number genotype for imprecise events\">\n";
	headerString+="##FORMAT=<ID=DV,Number=1,Type=Integer,Description=\"Number of paired-ends that support the event\">\n";
	headerString+="##FORMAT=<ID=RV,Number=1,Type=Integer,Description=\"Number of split reads that support the event\">\n";



	//this sting contains info such as insert size and mean coverage
	headerString+=libraryData+"\n";
	//Header
	headerString+="#CHROM\tPOS\tID\tREF\tALT\tQUAL\tFILTER\tINFO\tFORMAT\t";
	return(headerString);
}



Window::Window(string bamFileName, bool outtie, float meanCoverage,string outputFileHeader, map<string,int> SV_options) {
	this->max_insert		 = SV_options["max_insert"];
	this->minimum_mapping_quality = SV_options["mapping_quality"];
	this->outtie			 = outtie;
	this->mean_insert		 = SV_options["meanInsert"];
	this ->std_insert		 = SV_options["STDInsert"];
	this->minimumPairs		 = SV_options["pairs"];
	this->minimumSplits		 = SV_options["splits"];
	this->minimumVariantSize = SV_options["min_variant_size"];
	this->meanCoverage		 = meanCoverage;
	this->bamFileName		 = bamFileName;
	this -> ploidy           = SV_options["ploidy"];
	this -> readLength       = SV_options["readLength"];
	this -> pairOrientation				 = 0;          
	this->outputFileHeader     = outputFileHeader;


}

void Window::insertRead(BamAlignment alignment) {
	readStatus alignmentStatus = computeReadType(alignment, this->max_insert,this->min_insert, this->outtie);

	if( not alignment.IsMateMapped()  or alignment.MapQuality < minimum_mapping_quality or alignmentStatus == lowQualty) {
		return; // in case the alignment is of no use discard it
	}

	if(this->chr == -1) { //sets chr to the first chromosome at startup
		cout << "working on sequence " << position2contig[alignment.RefID] << "\n";
		chr=alignment.RefID;
	}

	if(alignment.RefID != chr) { // I am moving to a new chromosomes, need to check if the current window can be used or not
		cout << "working on seqence " << position2contig[alignment.RefID] << "\n";
	}
	
	bool alignment_split = false;	
	alignment.BuildCharData();			
	alignment_split = alignment.HasTag("SA");
	if (alignment_split and alignment.IsPrimaryAlignment() and not alignment.IsSupplementaryAlignment() ) {
		// parse split read to get the other segment position, akin to a mate.
		string SA;
		alignment.GetTag("SA",SA);
		
		stringstream ss(SA);
		std::string item;
		while (std::getline(ss, item, ';')) {
			stringstream SS(item);
			string SA_data;
			vector <string> SA_elements;
			while (std::getline(SS, SA_data, ',')) {
				SA_elements.push_back(SA_data);
			}
			


			int contigNr = contig2position[SA_elements[0]];
			int currrentAlignmentPos=alignment.Position;
			int discordantDistance=0;
			int splitDistance = 0;

			long splitPos = atol(SA_elements[1].c_str());
			std::stringstream ss;

			string orientationA="-";
			if( alignment.IsReverseStrand() == false  ){
				orientationA="+";
			}

			if(alignment.RefID == contigNr and splitPos < alignment.Position){
				ss << position2contig[alignment.RefID] << "\t" << alignment.Position +1 << "\t" << orientationA << "\t" << alignment.MapQuality << "\t" << SA_elements[0] << "\t" << splitPos+1 << "\t"<< SA_elements[2] << "\t" << SA_elements[4] << "\t" << 1 << "\n";
			}else if(alignment.RefID < contigNr){
				ss << position2contig[alignment.RefID] << "\t" << alignment.Position +1 << "\t" << orientationA << "\t" << alignment.MapQuality << "\t" << SA_elements[0] << "\t" << splitPos+1 << "\t"<< SA_elements[2] << "\t" << SA_elements[4] << "\t" << 1 <<"\n";
			}else{
				ss << SA_elements[0] << "\t" << splitPos+1 << "\t"<< SA_elements[2] << "\t"<< SA_elements[4] << "\t" << position2contig[alignment.RefID] << "\t" << alignment.Position +1 << "\t" << orientationA << "\t" << alignment.MapQuality << "\t" << 1 << "\n";
			}

			SV_calls[alignment.MateRefID].push_back(ss.str());
		}
	}
	
	if(alignmentStatus == pair_wrongChrs or alignmentStatus ==  pair_wrongDistance) {
		if(alignment.RefID < alignment.MateRefID or (alignment.RefID == alignment.MateRefID and alignment.Position < alignment.MatePosition)) {  // insert only "forward" variations

			string orientationA="-";
			if( alignment.IsReverseStrand() == false  ){
				orientationA="+";
			}

			string orientationB="-";
			if(alignment.IsMateReverseStrand() == false  ){
				orientationB="+";
			}


			std::stringstream ss;
			ss << position2contig[alignment.RefID] << "\t" << alignment.Position +1 << "\t" << orientationA << "\t" << alignment.MapQuality << "\t" << position2contig[alignment.MateRefID] << "\t" << alignment.MatePosition+1 << "\t"<< orientationB << "\t" << -1 << "\t" << 100 << "\n";
			SV_calls[alignment.MateRefID].push_back(ss.str());

		}

	}

	chr=alignment.RefID;
}



float Window::computeCoverageB(int chrB, int start, int end, int32_t secondWindowLength){
	float coverageB=0;
	return(coverageB);
	
}

float Window::computeRegionalQuality(int chrB, int start, int end,int bin_size){
	float coverageB=0;
	return(coverageB);
	
}

int Window::computeRegionalLinks(int chrB,int start, int end,int bin_size){
	int links=0;
	return(links);	
}

//This function accepts a queue with alignments that links to chrB from chrA, the function returns the number of reads that are positioned inside the region on A and have mates that are linking anywhere on B
vector<int> Window::findLinksToChr2(queue<BamAlignment> ReadQueue,long startChrA,long stopChrA,long startChrB,long endChrB, int pairsFormingLink){
	int linkNumber=0;
	int QueueSize=ReadQueue.size();
	int estimatedDistance=0;
	int lengthWindowA=stopChrA-startChrA+1;

	for(int i=0;i<QueueSize;i++){
		if(startChrA <= ReadQueue.front().Position and stopChrA >= ReadQueue.front().Position ){
			linkNumber+=1;
			//If the reads are bridging the two windows, calculate the mate and read distance
			if(startChrB <= ReadQueue.front().MatePosition and endChrB >= ReadQueue.front().MatePosition){
				estimatedDistance+=lengthWindowA-(ReadQueue.front().Position-startChrA)+(ReadQueue.front().MatePosition-startChrB);
			}
		}
		ReadQueue.pop();
	}
	vector<int> output;
	output.push_back(linkNumber);
	if(pairsFormingLink > 0){
		output.push_back(estimatedDistance/pairsFormingLink);	
	}else{
		//if no links exists, the region will not count as an event, thus any number may be returned as the distance(the statistics wont be printed anyway)
		output.push_back(-1);
	}
	return(output);
}

//Computes statstics such as coverage on the window of chromosome A
vector<double> Window::computeStatisticsA(string bamFileName, int chrB, int start, int end, int32_t WindowLength, string indexFile){
	queue<int> linksFromA=linksFromWin[this -> pairOrientation][chrB];
	vector<double> statisticsVector;
	int currentReadPosition=0;
	int linksFromWindow=0;
	while ( linksFromA.size() > 0 ) {
		currentReadPosition=linksFromA.front();
		linksFromA.pop();
		if(start-this->mean_insert <= currentReadPosition and end+this->mean_insert >= currentReadPosition){
				linksFromWindow+=1;
			}
	}
	linksFromWindow += computeRegionalLinks(this -> chr,start, end,100);
	//calculates the coverage and returns the coverage within the window
	double coverageA=computeCoverageB(this -> chr,start,end,WindowLength);
	statisticsVector.push_back(coverageA);statisticsVector.push_back(linksFromWindow);
	return(statisticsVector);
	
}



//Compute statistics of the variations and print them to file
bool Window::computeVariations(int chr2) {
	bool found = false;
	
	
	vector< vector< long > > discordantPairPositions;
	vector< vector< long > > splitReadPositions;
	vector< long > splitReadEndPositions;
	discordantPairPositions.resize(2);
	splitReadPositions.resize(2);
	bool discordantPairs=false;
	bool splitReads=false;
	queue <BamAlignment> test_queue = this-> eventReads[this -> pairOrientation][chr2];
	
	//transfer the positions of the split reads and discordant pairs into vectors
	if( test_queue.size() >= minimumPairs) {		
		while(test_queue.size() > 0 ){
			discordantPairPositions[0].push_back(test_queue.front().Position);
			discordantPairPositions[1].push_back(test_queue.front().MatePosition);
			test_queue.pop();
		}
		discordantPairs=true;
	}

	if(this->eventSplitReads[this -> pairOrientation][chr2].size() > 0){
		for (vector<BamAlignment>::iterator alnit = eventSplitReads[this -> pairOrientation][chr2].begin(); alnit != eventSplitReads[this -> pairOrientation][chr2].end(); ++alnit) {
			string SA;
			(alnit)->GetTag("SA",SA);
			/* From the VCF documentation: Other canonical
			// alignments in a chimeric alignment, formatted as
			// a semicolon-delimited list:
			/ (rname,pos,strand,CIGAR,mapQ,NM;)+.  Each element	
			// in the list represents a part of the chimeric
			// alignment.  Conventionally, at a supplementary
			// line, the first element points to the primary
			// line.
			*/	  
			//First split at ;, keep only the 0th elemnt
			vector <string> SA_elements;
			stringstream ss(SA);
			std::string item;
			while (std::getline(ss, item, ';')) {
				stringstream SS(item);
				string SA_data;
				while (std::getline(SS, SA_data, ',')) {
					SA_elements.push_back(SA_data);
				}
			}	
			for(int j=0; j< SA_elements.size(); j+=6) {
				string contig = SA_elements[j];
				string chr2name = this -> position2contig[chr2];
				int mate_contig_id = contig2position[contig];
				long pos = atol(SA_elements[j+1].c_str());
				if((alnit)-> Position <= pos or mate_contig_id > chr2){
					splitReadPositions[0].push_back((alnit)-> Position);
					splitReadPositions[1].push_back(pos);
					splitReadEndPositions.push_back((alnit) -> GetEndPosition());
				}else{
					splitReadPositions[1].push_back((alnit)-> Position);
					splitReadPositions[0].push_back(pos);
					splitReadEndPositions.push_back(-1);
				}

			}
		}
		splitReads=true;
	}
	
	if(discordantPairs){
		vector<long> chr2regions= findRegionOnB( discordantPairPositions[1] ,12*sqrt(mean_insert*2));
	
		for(int i=0;i < chr2regions.size()/3;i++){
			long startSecondWindow=chr2regions[i*3];
			long stopSecondWindow=chr2regions[i*3+1];
			long pairsFormingLink=chr2regions[i*3+2];
			


			//resize region so that it is just large enough to cover the reads that have mates in the present cluster
			vector<long> chrALimit=newChrALimit(discordantPairPositions,startSecondWindow,stopSecondWindow);					

			long startchrA=chrALimit[0];
			long stopchrA=chrALimit[1];


			int splitsFormingLink=0;
			for(int j=0;j< splitReadPositions[0].size();j++){
				if(startSecondWindow-12*sqrt(mean_insert*2) <= splitReadPositions[1][j] and splitReadPositions[1][j] <= stopSecondWindow+12*sqrt(mean_insert*2)){	
					if(startchrA-12*sqrt(mean_insert*2) <= splitReadPositions[0][j] and splitReadPositions[0][j] <= stopchrA+12*sqrt(mean_insert*2)){
						splitsFormingLink++;
					}			
				}			
			}
			
						
			if(pairsFormingLink+splitsFormingLink < minimumPairs) {
				continue;
			}
			
			vector<int> statsOnB=findLinksToChr2(eventReads[this -> pairOrientation][chr2],startchrA, stopchrA,startSecondWindow,stopSecondWindow,pairsFormingLink);
			int numLinksToChr2=statsOnB[0];
			int estimatedDistance=statsOnB[1];
			if(pairsFormingLink/double(numLinksToChr2) > 0.15){
				
				//compute the coverage on the region of chromosome B
				double coverageRealSecondWindow=computeCoverageB(chr2, startSecondWindow, stopSecondWindow, (stopSecondWindow-startSecondWindow+1) );
				double coverageMid=0;
				if(this -> chr == chr2){
					coverageMid=computeCoverageB(chr2, startchrA, stopSecondWindow, (startchrA-startSecondWindow+1) );
				}				
				double qualityB = computeRegionalQuality(chr2, startSecondWindow, stopSecondWindow,100);
				double qualityA = computeRegionalQuality(this -> chr, startchrA, stopchrA,100);
				//compute coverage on the region of chromosome A, as well as the number of discordant pairs within this region
				vector<double> statisticsFirstWindow =computeStatisticsA(bamFileName, chr2, startchrA, stopchrA, (stopchrA-startchrA), indexFile);
				double coverageRealFirstWindow	= statisticsFirstWindow[0];
				int linksFromWindow=int(statisticsFirstWindow[1]);

				//calculate the expected number of reads
				int secondWindowLength=this-> readLength+(stopSecondWindow-startSecondWindow+1);
				int firstWindowLength=+this-> readLength+stopchrA-startchrA;

				if(estimatedDistance > firstWindowLength) {
					estimatedDistance = estimatedDistance - firstWindowLength;
				}else{
					estimatedDistance = 1;
				}
				//the pairs spanning the first and second window must have normal insert size, otherwise they are discordant
				if(estimatedDistance + firstWindowLength < mean_insert){
				    estimatedDistance = mean_insert - firstWindowLength;
				}
				
				float expectedLinksInWindow = ExpectedLinks(firstWindowLength, secondWindowLength, estimatedDistance, mean_insert, std_insert, coverageRealFirstWindow, this -> readLength);
				float expectedLinksInWindow2 = ExpectedLinks(mean_insert+std_insert, mean_insert+std_insert, 0, mean_insert, std_insert, coverageRealFirstWindow, this -> readLength);					
				if (expectedLinksInWindow > 10*pairsFormingLink){
					//dont print the variant if it is pure garbage
					continue;
				}
					
				string svType = "BND";
				string GT="./1";
				string CN=".";
				if(this->chr == chr2) {
					vector<string> svVector=classification(chr2,startchrA, stopchrA,coverageRealFirstWindow,startSecondWindow, stopSecondWindow,coverageRealSecondWindow,this -> mean_insert,this -> std_insert,this -> 	outtie,coverageMid);
					svType=svVector[0];
					GT = svVector[3];
					CN = svVector[4];
				}
				//THese are the statistics used to define a variant detected by discordant pairs
				map<string,int> discordantPairStatistics;
				discordantPairStatistics["chrA"]=this -> chr;
				discordantPairStatistics["chrB"]=chr2;
				discordantPairStatistics["start"]=stopchrA+1;
				discordantPairStatistics["windowA_start"]=startchrA+1;
				discordantPairStatistics["windowA_end"]=stopchrA+1;
				discordantPairStatistics["end"]=startSecondWindow+1;
				discordantPairStatistics["windowB_start"]=startSecondWindow+1;
				discordantPairStatistics["windowB_end"]=stopSecondWindow+1;
				discordantPairStatistics["orientation"]=pairOrientation;
				discordantPairStatistics["links_window"]=linksFromWindow;
				discordantPairStatistics["links_window_B"]=computeRegionalLinks(chr2,startSecondWindow, stopSecondWindow, 100);
				discordantPairStatistics["links_chr"]=numLinksToChr2;
				discordantPairStatistics["links_event"]=pairsFormingLink;
				discordantPairStatistics["expected_links"]=abs(expectedLinksInWindow)/this -> ploidy;
				discordantPairStatistics["expected_links2"]=abs(expectedLinksInWindow2)/this -> ploidy;
				discordantPairStatistics["qualityB"]=qualityB;
				discordantPairStatistics["qualityA"]=qualityA;
				//These statistics define the variants detected by split reads
				map<string,int> splitReadStatistics;
				splitReadStatistics["chrA"]=-1;
				splitReadStatistics["chrB"]=-1;
				splitReadStatistics["start"]=-1;
				splitReadStatistics["end"]=-1;
				splitReadStatistics["split_reads"]=splitsFormingLink;
				splitReadStatistics["coverage_mid"]=-1;
				
				map<string,float> statistics_floats;
				statistics_floats["coverage_start"]=coverageRealFirstWindow;
				statistics_floats["coverage_end"]=coverageRealSecondWindow;
				statistics_floats["coverage_mid"]=coverageMid;
				
				
				VCFLine(statistics_floats,discordantPairStatistics,splitReadStatistics,svType,GT,CN);
				found = true;
			}
		}
	//we only check for intrachromosomal variants, oherwise we would detect discrodant pairs aswell
	}else if(chr2 == this -> chr and splitReads){
		vector<long> chr2regions= findRegionOnB( splitReadPositions[1],this ->readLength);
	
		for(int i=0;i < chr2regions.size()/3;i++){

			

			long startSecondWindow=chr2regions[i*3];
			long stopSecondWindow=chr2regions[i*3+1];
			long splitsFormingLink=chr2regions[i*3+2];
			

			if(splitsFormingLink < this->minimumSplits ) {
				continue;
			}
			
			//resize region so that it is just large enough to cover the reads that have mates in the present cluster
			vector<long> chrALimit=newChrALimit(splitReadPositions,startSecondWindow,stopSecondWindow);					


			long startchrA=chrALimit[0];
			long stopchrA=chrALimit[1];
			//The variant needs to be larger than the minimum size bp, but small enough not to be detected by discordnt pairs
			if( abs( stopSecondWindow-startchrA) < this->minimumVariantSize or abs( startSecondWindow-stopchrA )  > this->max_insert) {
				continue;
			}

			//update the end position of chrA
			for(int j=0;j< splitReadPositions[0].size();j++){
				if(startSecondWindow <= splitReadPositions[1][j] and splitReadPositions[1][j] <= stopSecondWindow){
					if(startchrA <= splitReadPositions[0][j] and splitReadPositions[0][j] <= stopchrA){
						if( splitReadEndPositions[j] != -1){
							stopchrA=splitReadEndPositions[j];
  							break;
						}
					}
				}
			}


			//compute the coverage on the region of chromosome B
			double coverageRealSecondWindow=computeCoverageB(chr2, startchrA, stopSecondWindow, (startchrA-startSecondWindow+1) );
			//compute coverage on the region of chromosome A, as well as the number of discordant pairs within this region
			//we need a model for determination of variant type based on split reads
			string svType = "BND";
			
			if(this -> pairOrientation == 0 or this -> pairOrientation == 3){
				svType = "INV";
			}else if(coverageRealSecondWindow > 1.25*meanCoverage){
				svType = "DUP";
			}else if(coverageRealSecondWindow < 0.75*meanCoverage){
				svType = "DEL";
			}

			string GT="./1";
			string CN=".";
			//THese are the statistics used to define a variant detected by discordant pairs
			map<string,int> discordantPairStatistics;
			discordantPairStatistics["chrA"]=-1;
			discordantPairStatistics["links_event"]=0;
			
			
			//compute coverage on the region of chromosome A, as well as the number of discordant pairs within this region
			vector<double> statisticsFirstWindow =computeStatisticsA(bamFileName, chr2, startchrA, stopchrA, (stopchrA-startchrA), indexFile);
			
			//These statistics define the variants detected by split reads
			map<string,int> splitReadStatistics;
			splitReadStatistics["chrA"]=this -> chr;
			splitReadStatistics["chrB"]=chr2;
			splitReadStatistics["windowA_start"]=startchrA+1;
			splitReadStatistics["windowA_end"]=stopchrA+1;
			splitReadStatistics["start"]=stopchrA+1;
			splitReadStatistics["windowB_start"]=startSecondWindow+1;
			splitReadStatistics["windowB_end"]=stopSecondWindow+1;
			splitReadStatistics["end"]=startSecondWindow+1;
			splitReadStatistics["split_reads"]=splitsFormingLink;
			splitReadStatistics["splitOrientation"]= this -> pairOrientation;
			splitReadStatistics["qualityA"]=computeRegionalQuality(this -> chr, startchrA, stopchrA,100);
			splitReadStatistics["qualityB"]=computeRegionalQuality(this -> chr, startSecondWindow, stopSecondWindow,100);
			splitReadStatistics["links_window"]=int(statisticsFirstWindow[1]);

			map<string,float> statistics_floats;
			statistics_floats["coverage_start"]=statisticsFirstWindow[0];
			statistics_floats["coverage_end"]=computeCoverageB(chr2, startSecondWindow, stopSecondWindow, (stopSecondWindow-startSecondWindow+1) );
			statistics_floats["coverage_mid"]=computeCoverageB(chr2,startchrA, stopSecondWindow, (stopSecondWindow+1-startchrA) );
			
			VCFLine(statistics_floats,discordantPairStatistics,splitReadStatistics,svType,GT,CN);
			found = true;
		}
		
	}
	return(found);

}

void Window::initTrans(SamHeader head,string libraryData) {
	uint32_t contigsNumber = 0;
	SamSequenceDictionary sequences  = head.Sequences;
	for(SamSequenceIterator sequence = sequences.Begin() ; sequence != sequences.End(); ++sequence) {
		this->contig2position[sequence->Name] = contigsNumber; // keep track of contig name and position in order to avoid problems when processing two libraries
		this->position2contig[contigsNumber]  = sequence->Name;
		this-> contig_ids.push_back(sequence->Name);
		this-> contig_length.push_back(sequence->Length);
		if(sequence->HasAssemblyID() == true){
			this-> contig_assembly.push_back(sequence->AssemblyID);
		}
		contigsNumber++;
	}
	string intra_chr_eventsVCF = outputFileHeader + ".signals.tab";
	this->TIDDITVCF.open(intra_chr_eventsVCF.c_str());
	if(head.HasReadGroups() == false){
		this->TIDDITVCF << VCFHeader(libraryData) << outputFileHeader << "\n";
	}else{
		SamReadGroupDictionary readGroups = head.ReadGroups;
		string SM = outputFileHeader;
		for(SamReadGroupIterator readGroup = readGroups.Begin() ; readGroup != readGroups.End(); ++readGroup) {
			if( readGroup -> HasSample() == true){
				SM= readGroup -> Sample;
				break;
			}
		}
		this->TIDDITVCF << VCFHeader(libraryData) << SM << "\n";
	}

}

//this is a function that takes a queue containing reads that are involved in an event and calculates the position of the event on chromosome B(the chromosome of the of the mates). The total number of reads
//inside the region is reported, aswell as the start and stop position on B
vector<long> Window::findRegionOnB( vector<long> mate_positions,int maxDistance){
	queue<long> linksToRegionQueue;	
	vector<long> eventRegionVector;
	//sort the mate positions
	sort(mate_positions.begin(), mate_positions.end(), sortMate);
	//finds the region of the event
	linksToRegionQueue.push(mate_positions[0]);
	for(int i=1; i  < mate_positions.size();i++){

		//if the current mate is close enough to the previous mate
		if( maxDistance >= (mate_positions[i]-linksToRegionQueue.back()) ){
			linksToRegionQueue.push(mate_positions[i]);

		}else{
			//if there are enough links beetween two regions, the region and the number of pairs are saved, and later on returned
			if(linksToRegionQueue.size() > 0  ) {
				//the front of the queue is the start position
				eventRegionVector.push_back(linksToRegionQueue.front());
				//the back is the end position
				eventRegionVector.push_back(linksToRegionQueue.back());
				//the length is the number of links to this event
				eventRegionVector.push_back(linksToRegionQueue.size());
			}
			//the queue is reset so that the next event may be found(if any)
			linksToRegionQueue=queue<long>();
			linksToRegionQueue.push(mate_positions[i]);
		}
		
	}
	//if the queue contain any event 
	if(linksToRegionQueue.size() > 0  ) {
		eventRegionVector.push_back(linksToRegionQueue.front());
		eventRegionVector.push_back(linksToRegionQueue.back());
		eventRegionVector.push_back(linksToRegionQueue.size());
	}
	
	return(eventRegionVector);
}

//this function resizes the region of CHRA so that it fits only around reads that are linking to an event on chrB
vector<long> Window::newChrALimit(vector< vector< long > > variantPositions,long Bstart,long Bend){
	vector<long> startStopPos;
	long startA=-1;
	long endA=-1;
	for(int i=0; i< variantPositions[0].size() ;i++){
		//Checks if the alignment if the mate of the current read is located inside the chrB region	
		if(variantPositions[1][i] >= Bstart and variantPositions[1][i] <= Bend){
		//resize the chr A region so that it fits around all the reads that have a mate inside region 
			if(variantPositions[0][i] <= startA or startA ==-1){
				startA=variantPositions[0][i];
			}
			if(variantPositions[0][i] >= endA or endA ==-1){
				endA=variantPositions[0][i];
			}

		}
	}
	startStopPos.push_back(startA);
	startStopPos.push_back(endA);
	return(startStopPos);
}

//function that prints the statistics to a vcf file
void Window::VCFLine(map<string,float> statistics_floats,map<string,int> discordantPairStatistics, map<string,int> splitReadStatistics,string svType,string GT, string CN){
	


		
	string filter = "PASS";
	string infoField;
	infoField= "SVTYPE=" + svType;
	int chrB = -1;
	int chrA = -1;
	int posA= -1;
	int posB= -1;
	 
	//if we have detected a variant using discordant pairs
	if (discordantPairStatistics["chrA"] != -1){
		string read1_orientation;
		string read2_orientation;
		if (discordantPairStatistics["orientation"] == 0){
			read1_orientation="Forward";
			read2_orientation="Forward";
		}else if (discordantPairStatistics["orientation"] == 1){
			read1_orientation="Forward";
			read2_orientation="Reverse";

		}else if (discordantPairStatistics["orientation"] == 2){
			read1_orientation="Reverse";
			read2_orientation="Forward";
				
		}else{
			read1_orientation="Reverse";
			read2_orientation="Reverse";
		}


		float ratio1=-1;
		float ratio2=-1;
		float stat_ratio=1;
		if(discordantPairStatistics["expected_links"] != 0){
			ratio1=(float)discordantPairStatistics["links_event"]/(float)discordantPairStatistics["expected_links"];
			stat_ratio=ratio1;

		}
		if(discordantPairStatistics["expected_links2"] != 0){
			ratio2=(float)discordantPairStatistics["links_event"]/(float)discordantPairStatistics["expected_links2"];
			if(ratio1 == -1){
				stat_ratio=ratio2;
			}
			if(ratio2 > 1.4 and GT == "./1"){
				GT="1/1";
			}else{
				GT="0/1";
			}
		}
		
		
		
		std::stringstream ss;
		
		ss << ";CHRA=" << position2contig[discordantPairStatistics["chrA"]] << ";WINA=" << discordantPairStatistics["windowA_start"] << "," << discordantPairStatistics["windowA_end"];
		ss << ";CHRB=" << position2contig[discordantPairStatistics["chrB"]] << ";WINB=" << discordantPairStatistics["windowB_start"] << "," << discordantPairStatistics["windowB_end"];
		if(svType !="BND"){
		ss << ";END=" << discordantPairStatistics["end"];
		} 
		ss << ";LFA=" << discordantPairStatistics["links_window"];
		ss << ";LCB="  << discordantPairStatistics["links_chr"] << ";LTE=" << discordantPairStatistics["links_event"] << ";LFB=" << discordantPairStatistics["links_window_B"] << ";COVA=" <<  statistics_floats["coverage_start"];
		ss << ";COVM=" << statistics_floats["coverage_mid"];
		ss << ";COVB=" << statistics_floats["coverage_end"] << ";OA=" << read1_orientation << ";OB=" << read2_orientation;
		ss << ";QUALA=" << discordantPairStatistics["qualityA"] << ";QUALB=" << discordantPairStatistics["qualityB"];

		ss << ";EL="  << discordantPairStatistics["expected_links"] << ";RATIO=" <<  ratio1;
		ss << ";EL2="  << discordantPairStatistics["expected_links2"] << ";RATIO2=" <<  ratio2;
		int stat_links=discordantPairStatistics["links_event"];
		if(discordantPairStatistics["links_event"] < discordantPairStatistics["links_window_B"]){
			stat_links=discordantPairStatistics["links_window_B"];
		}
		filter=filterFunction(stat_ratio,stat_links,discordantPairStatistics["links_window"],mean_insert,std_insert,statistics_floats["coverage_start"],statistics_floats["coverage_end"],meanCoverage, discordantPairStatistics["windowA_end"],discordantPairStatistics["windowB_start"],discordantPairStatistics["chrA"],discordantPairStatistics["chrB"],this -> ploidy );	

		
		infoField += ss.str();
		chrB= discordantPairStatistics["chrB"];
		chrA = discordantPairStatistics["chrA"];
		posA= discordantPairStatistics["start"];
		posB= discordantPairStatistics["end"];
	}
	

	if (splitReadStatistics["chrA"] != -1){
		string read1_orientation;
		string read2_orientation;
		if (splitReadStatistics["splitOrientation"] == 0){
			read1_orientation="Forward";
			read2_orientation="Reverse";
		}else if (splitReadStatistics["splitOrientation"] == 1){
			read1_orientation="Forward";
			read2_orientation="Forward";

		}else if (splitReadStatistics["splitOrientation"] == 2){
			read1_orientation="Reverse";
			read2_orientation="Reverse";
				
		}else{
			read1_orientation="Reverse";
			read2_orientation="Forward";
		}

		double RATIO =1;
		int E_links= statistics_floats["coverage_start"]/(double(this -> ploidy));
		if (statistics_floats["coverage_start"] > statistics_floats["coverage_end"] and statistics_floats["coverage_end"] > 0){
			E_links= statistics_floats["coverage_end"]/(double(this -> ploidy));
		}

		if(E_links > 0){
			RATIO=double(splitReadStatistics["split_reads"])/double(E_links);
			if(RATIO > 1.4){
				GT="1/1";
			}else{
				GT="0/1";
			}
		}
		filter=filterFunction(RATIO*2,splitReadStatistics["split_reads"],splitReadStatistics["links_window"],mean_insert,std_insert,statistics_floats["coverage_start"],statistics_floats["coverage_end"],meanCoverage,splitReadStatistics["windowA_end"],splitReadStatistics["windowB_start"],splitReadStatistics["chrA"],splitReadStatistics["chrB"],this -> ploidy);
		
		
		chrB= splitReadStatistics["chrB"];
		chrA = splitReadStatistics["chrA"];
		posA= splitReadStatistics["start"];
		posB= splitReadStatistics["end"];
		std::stringstream ss;
		if(svType != "BND"){
			ss << ";END=" << posB;
		}
		ss << ";WINA=" << splitReadStatistics["windowA_start"] << "," << splitReadStatistics["windowA_end"];
		ss << ";WINB=" << splitReadStatistics["windowB_start"] << "," << splitReadStatistics["windowB_end"];
		ss << ";COVA=" << statistics_floats["coverage_start"] << ";COVM=" << statistics_floats["coverage_mid"] << ";COVB=" << statistics_floats["coverage_end"];
		ss <<  ";QUALA=" << splitReadStatistics["qualityA"] << ";QUALB=" << splitReadStatistics["qualityB"];
		ss << ";OA=" << read1_orientation << ";OB=" << read2_orientation << ";LFA=" << splitReadStatistics["links_window"] ;
		ss << ";ER=" << E_links << ";RATIO=" << RATIO;
		infoField += ss.str();
	}
	
	//Print the variant
	this -> numberOfEvents++;
	if(chrA == chrB) {
		
		//TODO generate the info field as a string, instead of printing the separate variable directly to the file
		if(svType != "INS" and svType !="BND"){
			std::stringstream var;
			var << this -> position2contig[chrA]  << "\t" <<     posA   << "\tSV_" << this -> numberOfEvents << "_1" <<  "\t"  ;
			var << "N"       << "\t"	<< "<" << svType << ">";
			var << "\t.\t"  << filter  << "\t" << infoField;
			if (CN != int2str(this -> ploidy) ){
				var << "\tGT:CN:DV:RV\t" << GT << ":" << CN << ":" << discordantPairStatistics["links_event"] << ":" << splitReadStatistics["split_reads"] << "\n";
			}else{
				var << "\tGT:DV:RV\t" << GT << ":" << discordantPairStatistics["links_event"] << ":" << splitReadStatistics["split_reads"] << "\n";
			}

			SV_calls[chrA].push_back(var.str());
			vector<int> row;
			row.push_back(SV_calls[chrA].size()-1);
			row.push_back(posA);
			SV_positions[chrA].push_back(row);

		}else{
			std::stringstream bnd_st;
			bnd_st << position2contig[chrA]  << "\t" <<     posA   << "\tSV_" << this -> numberOfEvents << "_1" << "\t";
			bnd_st << "N"       << "\t"	<< "N[" << position2contig[chrB] << ":" << posB << "[";
			bnd_st << "\t.\t"  << filter  << "\t" << infoField;
			bnd_st << "\tGT:DV:RV\t" << GT << ":" << discordantPairStatistics["links_event"] << ":" << splitReadStatistics["split_reads"] << "\n";

			SV_calls[chrA].push_back(bnd_st.str());
			vector<int> row_st;
			row_st.push_back(SV_calls[chrA].size()-1);
			row_st.push_back(posA);
			SV_positions[chrA].push_back(row_st);


			//print the second breakend
			std::stringstream bnd_nd;
			bnd_nd <<  position2contig[chrB] << "\t" <<    posB    << "\tSV_" << this -> numberOfEvents <<  "_2" << "\t";
			bnd_nd << "N"       << "\t"	<< "N]" << position2contig[chrA]  << ":" << posA << "]";
			bnd_nd << "\t.\t"  << filter  << "\t" << infoField;
			bnd_nd << "\tGT:DV:RV\t" << GT << ":" << discordantPairStatistics["links_event"] << ":" << splitReadStatistics["split_reads"] << "\n";

			SV_calls[chrB].push_back(bnd_nd.str());
			vector<int> row_nd;
			row_nd.push_back(SV_calls[chrB].size()-1);
			row_nd.push_back(posB);
			SV_positions[chrB].push_back(row_nd);

		}

	} else {
		//print the first breakend
		std::stringstream bnd_st;
		bnd_st << position2contig[chrA]  << "\t" <<     posA   << "\tSV_" << this -> numberOfEvents << "_1" << "\t";
		bnd_st << "N"       << "\t"	<< "N[" << position2contig[chrB] << ":" << posB << "[";
		bnd_st << "\t.\t"  << filter  << "\t" << infoField;
		bnd_st << "\tGT:DV:RV\t" << GT << ":" << discordantPairStatistics["links_event"] << ":" << splitReadStatistics["split_reads"] << "\n";

		SV_calls[chrA].push_back(bnd_st.str());
		vector<int> row_st;
		row_st.push_back(SV_calls[chrA].size()-1);
		row_st.push_back(posA);
		SV_positions[chrA].push_back(row_st);

		//print the second breakend
		std::stringstream bnd_nd;
		bnd_nd <<   position2contig[chrB]<< "\t" <<    posB    << "\tSV_" << this -> numberOfEvents <<  "_2" << "\t";
		bnd_nd << "N"       << "\t"	<< "N]" << position2contig[chrA]  << ":" << posA << "]";
		bnd_nd << "\t.\t"  << filter  << "\t" << infoField;
		bnd_nd << "\tGT:DV:RV\t" << GT << ":" << discordantPairStatistics["links_event"] << ":" << splitReadStatistics["split_reads"] << "\n";


		vector<int> row_nd;
		row_nd.push_back(SV_calls[chrB].size()-1);
		row_nd.push_back(posB);
		SV_positions[chrB].push_back(row_nd);

	}
	return;

}
