#include "golay24.h"

golay24::golay24()
{
}

myVector golay24::code(myVector a){
    int aSize = (int) a.size();
    if(aSize%12 != 0){
        for(int i = 0; i < (12 - aSize%12); i++){
            a.push_back(0);
        }
    }
    // parity matrix
    int parity[12] = {	0x7ff, 0xee2, 0xdc5, 0xb8b, 0xf16, 0xe2d,
                        0xc5b, 0x8b7, 0x96e, 0xadc, 0xdb8, 0xb71 };
    // vector result
    myVector aResult;
    // obrabotka kazhdih 12 bitov
    for (int i = 0; i < a.size()/12; i++){
        myVector aPart( a, 12*i, 12*i+11);
        aResult.insert(aResult.end(), aPart.begin(), aPart.end());
        //*************** GENERATING OF PARITY WORD **********************************
        myVector aParity((int) aPart.size(),0);
        int aPartDec = aPart.Bin2Dec();
        int aParityDec = aParity.Bin2Dec();
        int aux;
        for (int i = 0; i<aParity.size(); i++){
            aux = 0;
            for (int j=0; j<aParity.size(); j++){
                aux = aux ^ ( (aPartDec&parity[i])>>j & 0x01);
            }
            aParityDec = (aParityDec << 1) ^ aux;
        }
        aParity.Dec2Bin(aParityDec);
        aResult.insert(aResult.end(), aParity.begin(), aParity.end());
    }
    return aResult;
}


myVector golay24::decode(myVector a)
{
    // identity matrix
    int identity[12] = {0x800, 0x400, 0x200, 0x100, 0x080, 0x040,
                        0x020, 0x010, 0x008, 0x004, 0x002, 0x001 };
    // parity matrix
    int parity[12] = {	0x7ff, 0xee2, 0xdc5, 0xb8b, 0xf16, 0xe2d,
                        0xc5b, 0x8b7, 0x96e, 0xadc, 0xdb8, 0xb71 };
    int syndrome;						// syndrome
    int modified_syndrome;				// modified syndrome
    int aux, found;						// division variables

    myVector aResult;
    // obrabotka kazhdih 24 bitov
    for (int m = 0; m < a.size()/24; m++){
        int received[2];
        int error[2];
        int codeword_estimated[2];
        myVector aCodeword(a, 24*m, (24*m+11) );
        myVector aParity(a, (24*m+11), (24*m+23) );
        // ---------------- Decoder ---------
        received[0] = aCodeword.Bin2Dec();
        received[1] = aParity.Bin2Dec();
        // Step 1: Compute the syndrome of the received vector
        syndrome = 0;
        for (int j=0; j<12; j++){

            aux = 0;
            for (int i=0; i<12; i++){
                aux = aux ^ ( (identity[j]&received[0])>>i &0x01 ); // identity part
            }
            for (int i=0; i<12; i++){
                aux = aux ^ ( (parity[j]&received[1])>>i &0x01 ); // parity part
            }
            syndrome = (syndrome<<1)^aux;
        }
        //Step 2
        if (weight(syndrome)<=3){
            error[0] = syndrome;
            error[1] = 0;
        } else {
            //Step 3
            int i = 0;
            found = 0;
            do {
                if (weight(syndrome^parity[i]) <=2){
                    error[0] = syndrome^parity[i];
                    error[1] = identity[i];
                    found = 1;
                }
                i++;
            } while ( (i<12) && (!found) );
            if (( i==12 ) && (!found)){
                //Step 4
                modified_syndrome = 0;
                for (int j=0; j<12; j++){
                    aux = 0;
                    for (int i=0; i<12; i++){
                        aux = aux ^ ( (parity[j]&syndrome)>>i & 0x01 );
                    }
                    modified_syndrome = (modified_syndrome<<1) ^ aux;
                }
                //Step 5
                if (weight(modified_syndrome) <=3){
                    error[0] = 0;
                    error[1] = modified_syndrome;
                } else {
                    //Step 6
                    int i = 0;
                    found = 0;
                    do {
                        if (weight(modified_syndrome^parity[i]) <=2){
                            error[0] = identity[i];
                            error[1] = modified_syndrome^parity[i];
                            found = 1;
                        }
                        i++;
                    } while ( (i<12) && (!found) );
                    if ((i==12) && (!found)){
                        //Step 7
                        //printf("Error : uncorrectable error pattern\n");
                    }
                }
            }
        }
        //* ------------------- Correct received word ---------------------
        codeword_estimated[0] = received[0]^error[0];
        codeword_estimated[1] = received[1]^error[1];
        myVector codeword_estimated_0_vector(12,0);
        codeword_estimated_0_vector.Dec2Bin(codeword_estimated[0]);
        aResult.insert(aResult.end(), codeword_estimated_0_vector.begin(), codeword_estimated_0_vector.end());
        codeword_estimated_0_vector.clear();
        // ----------------------------------
    }
    return aResult;
}

int golay24::weight(int intVector){
    int aux = 0;
    for (int i=0; i<12; i++){
        if ( (intVector>>i)&1 ){
            aux++;
        }
    }
    return(aux);
}
