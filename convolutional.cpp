#include "convolutional.h"

convolutional::convolutional()
{
    /*  default
         *
         *  2 2
         *  5
         *  7
         *
         */

    this->k2 = 1;
    this->n2 = 2;
    this->m2 = 2;
    this->TRUNC_LENGTH = 5*this->m2 + 1;
    this->g2[0][0] = 5;
    this->g2[1][0] = 7;
}



void convolutional::set_TRUNC_LENGTH(long aValue){
    this->TRUNC_LENGTH = aValue;
}

void convolutional::set_k2(int aValue){
    this->k2 = aValue;
}

void convolutional::set_n2(int aValue){
    this->n2 = aValue;
}

void convolutional::set_m2(int aValue){
    this->m2 = aValue;
}


long convolutional::get_TRUNC_LENGTH(){
    return this->TRUNC_LENGTH;
}

int convolutional::get_k2(){
    return this->k2;
}

int convolutional::get_n2(){
    return this->n2;
}

int convolutional::get_m2(){
    return this->m2;
}

void convolutional::set_g2(QStringList new_g2){
    for(int i = 0; i < this->n2; i++ ){
        this->g2[i][0] = new_g2.at(i).toInt();
    }
}

QStringList convolutional::get_g2(){
    QStringList result;
    for(int i = 0; i < this->n2; i++){
        result.append(QString::number(g2[i][0]));
    }
    return result;
}

QStringList convolutional::getInfo(){
    QStringList result;
    result.append("Number of input bites k2 = " + QString::number(this->k2));
    result.append("Number of output bites n2 = " + QString::number(this->n2));
    result.append("Memory m2 = " + QString::number(this->m2));
    result.append("TRUNC_LENGTH (for decoder)= " + QString::number(this->TRUNC_LENGTH));

    for(int i = 0; i < this->n2; i++){
        result.append(QString::number(i+1) + " polynom: " + QString::number(g2[i][0]));
    }
    return result;
}

myVector convolutional::decode(myVector a)
{
    trellis();
    myVector aResult;
    register int i, j, k;
    vector<vector<trel> > trellis(1024, vector<trel>(100));
    vector<surv> survivor(1024);
    vector<surv> surv_temp(1024);
    /* =================== READ TRELLIS STRUCTURE ==================== */
    for (j=0; j<this->NUM_STATES; j++){
        for (k=0; k<this->NUM_TRANS; k++){
            myVector aTrellisLine = this->aTrellis.at(j*2+k);
            trellis[j][k].init = aTrellisLine.at(0);
            trellis[j][k].data = aTrellisLine.at(1);
            trellis[j][k].final = aTrellisLine.at(2);
            for (i=0; i < this->OUT_SYM; i++){
                trellis[j][k].output[i] = aTrellisLine.at(3+i);
            }
        }
    }
    /* Initialize survivor sequences and metrics */
    for (i=0; i<this->NUM_STATES; i++){
        survivor[i].metric = 0.0;             /* Metric = 0 */
        for (j=0; j<this->TRUNC_LENGTH; j++){
            survivor[i].data[j] = 0;        /* Estimated data = 0 */
            survivor[i].state[j] = 0;       /* Estimated state = 0 */
        }
    }
    vector<unsigned int> data_symbol;
    for (i=0; i<this->TRUNC_LENGTH; i++)
    {
        data_symbol.push_back(0);
    }
    /* Index used in simulation loop */
    int index = 0;
    int received[100];
    /* ===================== SIMULATION LOOP ========================= */
    while (index < a.size()/this->OUT_SYM)
    {

        /* ADD ADDITIVE WHITE GAUSSIAN NOISE */
        int temp_int = index*this->OUT_SYM;
        for (i=0; i<this->OUT_SYM; i++)
        {
            received[i] = a.at(temp_int + i);
        }

       //===================================
        /* VITERBI DECODE */
        double aux_metric;
        vector<double> surv_metric(this->NUM_STATES);
        register int k;

        for (i=0; i<this->NUM_STATES; i++) /* Initialize survivor branch metric */
            surv_metric[i] = -DBL_MAX;

        for (i=0; i<this->NUM_STATES; i++) /* Loop over inital states */
        {
            for (j=0; j<this->NUM_TRANS; j++) /* Loop over data */
            {

                /* Compute CORRELATION between received seq. and coded branch */
                aux_metric = 0.0;
                for (k=(this->OUT_SYM-1); k>=0; k--)
                    aux_metric += this->comp_metric(received[k],trellis[i][j].output[k]);
                aux_metric += survivor[i].metric;

                /* compare with survivor metric at final state */
                /* We compare CORRELATIONS */

                if ( aux_metric > surv_metric[trellis[i][j].final] )
                { /* Good candidate found */
                    surv_metric[trellis[i][j].final] = aux_metric;

                    /* Update data and state paths */
                    for (k=0; k<this->TRUNC_LENGTH; k++)
                    {
                        surv_temp[trellis[i][j].final].data[k] =
                                survivor[i].data[k];
                        surv_temp[trellis[i][j].final].state[k] =
                                survivor[i].state[k];
                    }
                    surv_temp[trellis[i][j].final].data[index%this->TRUNC_LENGTH] = j;
                    surv_temp[trellis[i][j].final].state[index%this->TRUNC_LENGTH] =
                            trellis[i][j].final;

                }
            }
        }

        for (i=0; i<this->NUM_STATES; i++)
        {
            /* Update survivor metrics */
            survivor[i].metric = surv_metric[i];
            for (k=0; k<this->TRUNC_LENGTH; k++)
            {
                survivor[i].data[k] = surv_temp[i].data[k];
                  survivor[i].state[k] = surv_temp[i].state[k];
            }
        }
        //******************************************************
        if ((index % this->TRUNC_LENGTH == this->TRUNC_LENGTH - 1) || (index == a.size()/this->OUT_SYM - 1)){
            int maxMetricIndex = -1;
            int maxMetric = -1;
            for (i=0; i<this->NUM_STATES; i++)
            {
                if(survivor[i].metric >= maxMetric){
                    maxMetric = survivor[i].metric;
                    maxMetricIndex = i;
                }
            }
            if ((index % this->TRUNC_LENGTH == this->TRUNC_LENGTH - 1)){
                for (k=0; k<this->TRUNC_LENGTH; k++)
                {
                    aResult.push_back(survivor[maxMetricIndex].data[k]);
                }
            } else {
                for (k=0; k<=index % this->TRUNC_LENGTH; k++)
                {
                    aResult.push_back(survivor[maxMetricIndex].data[k]);
                }

            }
        }
        //******************************************************
        index += 1;           /* Increase simulation index */
    }
    return aResult;
}

vector<myVector> convolutional::get_trellis()
{
    return this->aTrellis;
}

myVector convolutional::code(myVector a){
    trellis();
    long index = 0;
    this->memory2 = 0;
    myVector aResult;

    while (index < a.size())
    {
        // Binary convolutional encoder, rate 1/n2
        register int i, j, result, temp;

        temp = this->memory2;
        this->output = 0;

        temp = (temp<<1) ^ a.at(index % a.size());

        for (i=0; i<this->n2; i++)
        {
            result = 0;
            for (j=this->m2; j>=0; j--){
                result ^= ( ( temp & this->g2[i][0] ) >> j ) & 1;
            }
            this->output = ( this->output<<1 ) ^ result;
        }
        myVector convert;
        convert.resize(this->n2);
        convert.Dec2Bin(output);
        aResult.insert(aResult.end(),convert.begin(),convert.end());


        this->memory2 = temp ;
        index++;
    }
    return aResult;
}

void convolutional::trellis()
{
    register int i, j, k, signal;



    this->NUM_STATES = 1;
    for (i=0;i<this->m2;i++){
        this->NUM_STATES *= 2;
    }
    this->OUT_SYM = this->n2;
    this->NUM_TRANS = 1;
    for (i=0;i<this->k2;i++){
        this->NUM_TRANS *= 2;
    }
    // open file 2
    myVector temp;
    k = this->OUT_SYM - 1;

    for (this->memory2=0; this->memory2 < this->NUM_STATES; this->memory2++){
        for (this->data2=0; this->data2<2; this->data2++){
            temp.push_back(this->memory2);
            temp.push_back(this->data2);
            register int result, ttemp;

            ttemp = this->memory2;
            this->output = 0;
            ttemp = (ttemp<<1) ^ ( this->data2 & 0x01 );
            for (i=0; i<this->n2; i++)
            {
                result = 0;
                for (j=this->m2; j>=0; j--){
                    result ^= ( ( ttemp & this->g2[i][0] ) >> j ) & 1;
                }
                this->output = ( this->output<<1 ) ^ result;
            }
            this->state = ttemp & (this->NUM_STATES-1);
            temp.push_back(this->state);
            // Convert output to +1,-1 format
            for (i=k; i >=0; i--){
                if ( (this->output >> i) & 1 ){
                    signal = /*-1*/1;  // if bit = 1
                } else {
                    signal = /*+1*/0;  // if bit = 0
                }
                temp.push_back(signal);

            }
            this->aTrellis.push_back(temp);
            temp.clear();
        }
    }
}


double convolutional::comp_metric(double rec, double ref)
{ /* CORRELATION between received and reference values, to reduce the
         computational effort */
    if (ref == 0){
        if(rec == 1){
            return(-1);
        } else {
            return(1);
        }
    } else {
        if(rec == 1){
            return(1);
        } else {
            return(-1);
        }
    }
}
