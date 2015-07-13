/*
 * processCSV.cpp
 *
 * TODO: rename to scadaModbus
 * TODO: add mac address
 *
 * MALIPHOL
 * 7/2015
 *
 * Merge MODBUS/TCP request/response transactions from pcap
 * file that has captured NORMAL MODBUS/TCP network traffic,
 * ie, there are two packets per mbtcp transaction ID, one
 * request, followed by a response.
 *
 * STEP1- merge transactions by mbtcp transaction id from pcap
 * STEP2- create csv file with merged transactions
 *
 * Compile:
 *   g++ processCSV.cpp -o processCSV -std=c++11 -lpthread
 *
 * TODO x make precision longer in time vars
 *
 */

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>

#include "lib/csv.h"

using namespace std;
using namespace io;


// ********************* VARIABLES ********************* 

// debug
const bool check = 1;
//const bool check = 0;

//const string FILENAME = "mb2k.dat";
string FILENAME;
//const string CSVFILE  = "test.csv";
string CSVFILE;

// number of variables in packet
const int packetLen = 18;

// packet
struct packetStr {
	int    frameNumber;
	double frameTimeRel;
    double frameTimeDeltaDisplay;
	int    frameLen;
	char   ipProto;
    char   ipVersion;
    string ipSrc;
	string ethSrc;
	string ipDst;
	string ethDst;
	string mbtcpModbusUnitId;
	string tcpSrcPort;
	string tcpDstPort;
	char   mbtcpProtId;
	int    mbtcpTransId;
	int    mbtcpLen;
	string mbtcpModbusFuncCode;
	string mbtcpModbusRefNum;
	int    mbtcpModbusWordCnt;
	string mbtcpModbusData;
} packet, prevPacket;

// merged mbtcp transaction
struct mbtcpTransStr {
	int    frameNumber;
	double frameTimeRel;
    double frameTimeDeltaDisplay;
	int    frameLen;
	char   ipProto; //
    char   ipVersion; //
    string ipSrc;
	string ethSrc;
	string ipDst;
	string ethDst;
	string mbtcpModbusUnitId;
	string tcpSrcPort;
	string tcpDstPort;
	char   mbtcpProtId;
	int    mbtcpTransId;
	int    mbtcpLen;
	string mbtcpModbusFuncCode;
	string mbtcpModbusRefNum;
	int    mbtcpModbusWordCnt;
    int    respFrNumber;
    double respTimeRel;
    double respTimeDelta;
    int    respLen;
    string respIpSrc;
    string respEthSrc;
    string respIpDest;
    string respEthDest;
    string respUnitId;
    string respSrcport;
    string respDstPort;
    int    respProtId;
    int    respTransId;
    string respMbtcpLen;
    string respFuncCode;
	string mbtcpModbusData;
	int    d;
} mbtcpTrans;

// all merged mbtcp transactions
vector<mbtcpTransStr> mergedTrans;


// ********************* FUNCTIONS ********************* 

// convert hexidecimal string to decimal
int hexToDec(string str) {

	string decS;
	char delim[] = ":";
	char *token = strtok( &str[0], delim );

	while (token != NULL ) {

		decS += token;
		token = strtok( NULL, delim );
	}

	return strtol( decS.c_str(), NULL, 16 );
}

// merge transactions from pcap file
int mergeTrans() {

	try {

		cout<<"STEP1: Processing file - " + FILENAME + "\n";

		CSVReader<packetLen> in( FILENAME.c_str() );
		while( in.read_row(
				   packet.frameNumber, packet.frameTimeRel, packet.frameTimeDeltaDisplay,
				   packet.frameLen, packet.ipProto, packet.ipVersion, packet.ipSrc, packet.ethSrc,
				   packet.ipDst, packet.ethDst, packet.mbtcpModbusUnitId, packet.tcpSrcPort,
				   packet.tcpDstPort, packet.mbtcpProtId, packet.mbtcpTransId, packet.mbtcpLen,
				   packet.mbtcpModbusFuncCode, packet.mbtcpModbusRefNum, packet.mbtcpModbusWordCnt,
				   packet.mbtcpModbusData 
				   ) ) {

			// merge request and response packets
			if( packet.mbtcpTransId == prevPacket.mbtcpTransId) {
				
				//cout<<"mbtcpTransId: "<< packet.mbtcpTransId<<"\n";
				// request packet has destination port 502
				if( prevPacket.tcpDstPort == "502" ) {
					//cout <<"   Frame request: "<< prevPacket.frameNumber <<"\n";

					mbtcpTrans.frameNumber  = prevPacket.frameNumber;
					mbtcpTrans.frameTimeRel = prevPacket.frameTimeRel;
					mbtcpTrans.frameTimeDeltaDisplay = prevPacket.frameTimeDeltaDisplay;
					mbtcpTrans.frameLen     = prevPacket.frameLen;
					mbtcpTrans.ipProto      = prevPacket.ipProto;
					mbtcpTrans.ipVersion    = prevPacket.ipVersion;
					mbtcpTrans.ipSrc        = prevPacket.ipSrc;
					mbtcpTrans.ethSrc       = prevPacket.ethSrc;
					mbtcpTrans.ipDst        = prevPacket.ipDst;
					mbtcpTrans.ethDst       = prevPacket.ethDst;
					mbtcpTrans.mbtcpModbusUnitId = prevPacket.mbtcpModbusUnitId;
					mbtcpTrans.tcpSrcPort   = prevPacket.tcpSrcPort;
					mbtcpTrans.tcpDstPort   = prevPacket.tcpDstPort;
					mbtcpTrans.mbtcpProtId  = prevPacket.mbtcpProtId;
					mbtcpTrans.mbtcpTransId = prevPacket.mbtcpTransId;
					mbtcpTrans.mbtcpLen     = prevPacket.mbtcpLen;
					mbtcpTrans.mbtcpModbusFuncCode = prevPacket.mbtcpModbusFuncCode;
					mbtcpTrans.mbtcpModbusRefNum   = prevPacket.mbtcpModbusRefNum;
					mbtcpTrans.mbtcpModbusWordCnt  = prevPacket.mbtcpModbusWordCnt;
				}
				
				// response packet has source port 502
				if( packet.tcpSrcPort == "502" ) {
					//cout<<"   Frame response: "<< packet.frameNumber<< "\n";
					
					mbtcpTrans.respFrNumber = packet.frameNumber;
					mbtcpTrans.respTimeRel  = packet.frameTimeRel;
					mbtcpTrans.respTimeDelta = packet.frameTimeDeltaDisplay;
					mbtcpTrans.respLen       = packet.frameLen;
					mbtcpTrans.respIpSrc     = packet.ipSrc;
					mbtcpTrans.respEthSrc    = packet.ethSrc;
					mbtcpTrans.respIpDest    = packet.ipDst;
					mbtcpTrans.respEthDest   = packet.ethDst;
					mbtcpTrans.respUnitId    = packet.mbtcpModbusUnitId;
					mbtcpTrans.respSrcport   = packet.tcpSrcPort;
					mbtcpTrans.respDstPort   = packet.tcpDstPort;
					mbtcpTrans.respDstPort   = packet.tcpDstPort;
					mbtcpTrans.respProtId    = packet.mbtcpProtId; // xxxx
					mbtcpTrans.respTransId   = packet.mbtcpTransId;
					mbtcpTrans.respMbtcpLen  = packet.mbtcpLen;
					mbtcpTrans.respFuncCode  = packet.mbtcpModbusFuncCode;
					mbtcpTrans.mbtcpModbusData = packet.mbtcpModbusData; 
					mbtcpTrans.d = hexToDec(packet.mbtcpModbusData); 
					
					// add to all merged transactions
					mergedTrans.push_back(mbtcpTrans);
					
					// re-initialize transaction
					mbtcpTrans = (const struct mbtcpTransStr){ 0 };

				} // end if response

			} // end if same transaction id
			
			prevPacket = packet;
			
		} // end while read_row

		// check
		if (check) {
			
			printf("number of merged trans: %d\n", mergedTrans.size());;
			cout<<"checking....\n";
			
			for( auto &i : mergedTrans ) {
				cout<< i.frameNumber << endl;
				cout<< i.frameTimeRel << endl;
				cout<< i.frameLen << endl;
				cout<< i.ipProto << endl; //
				cout<< i.ipVersion << endl; //
				cout<< i.ipSrc << endl;
				cout<< i.ipDst << endl;
				cout<< i.mbtcpModbusUnitId << endl;
				cout<< i.tcpSrcPort << endl;
				cout<< i.tcpDstPort << endl;
				cout<< i.mbtcpProtId << endl;
				cout<< i.mbtcpTransId << endl;
				cout<< i.mbtcpLen << endl;
				cout<< i.mbtcpModbusFuncCode << endl;
				cout<< i.mbtcpModbusRefNum << endl;
				cout<< i.mbtcpModbusWordCnt << endl;
				cout<< i.respFrNumber << endl;
				cout<< i.respTimeRel << endl;
				cout<< i.respTimeDelta << endl;
				cout<< i.respLen << endl;
				cout<< i.respIpSrc << endl;
				cout<< i.respIpDest << endl;
				cout<< i.respEthDest << endl;
				cout<< i.respUnitId << endl;
				cout<< i.respSrcport << endl;
				cout<< i.respDstPort << endl;
				cout<< i.respProtId << endl;
				cout<< i.respTransId << endl;
				cout<< i.respMbtcpLen << endl;
				cout<< i.respFuncCode << endl;
				cout<< i.mbtcpModbusData << endl;
				cout<< i.d;
			}
		} // end check

	} catch (exception& e) {

		cout << "Exception mergeTrans() :\n" << endl;
		cout << e.what() << "\n" << endl;

	} // end try/catch

	return 0;

} // end mergeTrans()

// create CSV file
int createCSV() {

	cout<<"STEP2: Creating CSV file - " + CSVFILE + "\n";

	ofstream csvfile;
	csvfile.open(CSVFILE);

	// write merged transactions to CSV file
	for( auto &i : mergedTrans ) {

		cout<< i.frameNumber << endl;

		csvfile << "," << i.frameNumber;
		csvfile << "," << i.frameTimeRel; //x
		csvfile << "," << i.frameTimeDeltaDisplay; //x
		csvfile << "," << i.frameLen;
		csvfile << "," << i.ipProto;
		csvfile << "," << i.ipVersion;
		csvfile << "," << i.ipSrc;
		csvfile << "," << i.ipDst;
		csvfile << "," << i.mbtcpModbusUnitId;
		csvfile << "," << i.tcpSrcPort;
		csvfile << "," << i.tcpDstPort;
		csvfile << "," << i.mbtcpProtId;
		csvfile << "," << i.mbtcpTransId;
		csvfile << "," << i.mbtcpLen;
		csvfile << "," << i.mbtcpModbusFuncCode;
		csvfile << "," << i.mbtcpModbusRefNum;
		csvfile << "," << i.mbtcpModbusWordCnt;
		csvfile << "," << i.respFrNumber;
		csvfile << "," << i.respTimeRel; //x
		csvfile << "," << i.respTimeDelta; //x
		csvfile << "," << i.respLen;
		csvfile << "," << i.respIpSrc;
		csvfile << "," << i.respIpDest;
		csvfile << "," << i.respEthDest;
		csvfile << "," << i.respUnitId;
		csvfile << "," << i.respSrcport;
		csvfile << "," << i.respDstPort;
		cout<< "   protid : " << i.respProtId<<"\n";
		csvfile << "," << i.respProtId; // x something wrong here
		csvfile << "," << i.respTransId;
		csvfile << "," << i.respMbtcpLen;
		csvfile << "," << i.respFuncCode;
		csvfile << "," << i.mbtcpModbusData;
		csvfile << "," << i.d;
		csvfile << "\n";

	} // end for mergedTrans

	csvfile.close();

	return 0;

} // createCSV()

// ************************ MAIN ************************

int main( int argc, char *argv[] ) {

	if ( argc != 3 ) {

		cout<< "usage: " << argv[0] << " <datafile> <outfile>\n";

	} else {

		FILENAME = argv[1];
		CSVFILE  = argv[2];

		// STEP1
		mergeTrans();

		// STEP2
		createCSV();
	}

} // end main
