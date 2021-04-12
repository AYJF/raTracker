/*
 * Copyright (c) 2013 Jimenez Fernandez Y.Alvaro
 * Copyright (c) 2013 Cruz Rodriguez C.Roberto
 *
 */

#include "trackerthread.h"
#include "global.h"
#include "Guid.h"
#include <cstdio>/*
#include <SETUPAPI.H>
#include <SetupAPI.h>*/
#include <QTime>
#include <QMessageBox>

#include <QDebug>

#define Freq_Cons_Ang  0.00725  //Constanst frequency of elevation
#define Freq_Cons_Bang 0.0198   //Constanst frequency of azimuth
#define MinFreq       0.26      //Smallest frequency of start (rampa)
#define MaxFreqAng   10.0       //Max frequency in elevation (10KHz)
#define MaxFreqBang1 15.0       //Max frequency in azimuth1 (15KHz)
#define MaxFreqBang2 25.0       //Max frequency in azimuth2 (25KHz)
#define MaxFreqBang3 30.0       //Max frequency in azimuth3 (30KHz)

double offset_an,offset_bang;
double g_angle,g_angle0;
double fAngOff0,fBangOff0;
int   commandcoun;
int cFLAG;
int maxcount;
bool b50msec;
bool bAngle,bMem,bBangDirection;
double maxElev,maxAzi;
double MinAng, MaxAng,MinBang,MaxBang1,MaxBang2,MaxBang3;
bool bAziControlSkip,bEleControlSkip;
double grayCode2Angle[0x2000];

/******************ABOUT initAll()********************
* The encoder provide the position of antenna in gray code
* not in angle.
* Convert angle to gray code is easy, use  the formula:
*               gc = bn^(bn>>1)
* The inverse process is more complicated,
* the solution of problem he comes given for storage the
* fraction of angles in the order of gray code (gc).
* Thus when obtaining one gray code, apply grayCode2Angle[gc]
* and We obtain the angle (0 - 360).
*****************************************************/
void initAll()
{

    ushort gc;
    for (ushort bn=0; bn<0x2000; ++bn){ //0x2000 (8192) steep of encoder is 0.0439453
        gc=bn^(bn>>1);
        grayCode2Angle[gc] = 360.0*double(bn)/0x2000;
    }
}

enum _track{tWait,tRun};

TrackerThread::TrackerThread(QObject *parent) :
    QThread(parent)
{

    m_nPoints=0;
    m_index=0;
    m_aziArray=0;
    m_eleArray=0;

    bang_off = 36.27;    //offset of azimut
    an_off   = 359.64;   //offset of elevation


    initAll();
}

void TrackerThread::run()
{
    MaxElevAnzi();
    emit setElevMax(maxElev);

    g_angle0 = maxAzi - 100;

    if(g_angle0 < 0.0)
        g_angle0 += 360;

    if((g_angle0<90. && g_angle0>-90.) || g_angle0>270 )
        bAngle = true;
    else
        bAngle = false;

    if((m_aziArray[0]<90. && m_aziArray[0]>-90.) || m_aziArray[0]>270. )
        bMem = true;
    else
        bMem = false;

    if(((maxAzi>0.0)&&(maxAzi< 180.))|| maxAzi<-180. )
        bBangDirection = true;
    else
        bBangDirection = false;

    if(bAngle)
        g_angle = g_angle0;
    else
    {
        if(bMem)
            g_angle = g_angle0 - 150;
        else
            g_angle = g_angle0 + 210;
    }

    if(m_ddk.OnFine(GUID_DEVINTERFACE_PCIA))//find the PCI device
    {
        DWORD Vender;
        m_ddk.OnCofigRead(0,2,&Vender); //if stay, cofigurate
    } else{
        emit toMessage(); //else, emit to warning
    }

    MinAng = Freq_Cons_Ang*MinFreq;//Smallest speed in elevation
    MaxAng = Freq_Cons_Ang*MaxFreqAng;//Max speed in elevation

    MinBang = Freq_Cons_Bang*MinFreq;//Smallest speed in azimuth
    MaxBang1 = Freq_Cons_Bang*MaxFreqBang1;//Max speed 1 in azimuth
    MaxBang2 = Freq_Cons_Bang*MaxFreqBang2;//Max speed 2 in azimuth
    MaxBang3 = Freq_Cons_Bang*MaxFreqBang3;//Max speed 3 in azimuth


    qWarning()<<"Ele "<<MinAng<<MaxAng;
    qWarning()<<"Azi "<<MinBang<<MaxBang1<<MaxBang2<<MaxBang3;

    fAngOff0 = fBangOff0 = 0.0;

    bAziControlSkip = bEleControlSkip = false;

    cFLAG = tWait;

    time_t ddSecs;
    commandcoun = 0;
    int b50ms, last50ms=0;
    double satVel;

    m_lastEleVelAnt=0; m_aziC=0;
    m_lastAziVelAnt=0; m_eleC=0;
    m_aziOffset = 0;
    m_eleOffset = 0;
    m_isRunning = true;
    while (m_isRunning && m_index < m_nPoints){

        ddSecs      = time(0);//current time
        b50ms = (QTime::currentTime().msec())/50;//count of 50 milliseconds
        b50msec = (b50ms%2); //1(true)-> stay in 50 milliseconds, 0(false)-> other case
        m_index = (ddSecs - m_initDate)*10;//differencs current time and begin of pass(in sec)
        m_index += (b50ms/2);  //count of milliseconds pass

        //m_index index of array elev and azi of satellite

        cFLAG = (m_index < 0) ? tWait : tRun;

        if ( b50ms != last50ms ){ //if pass 50 milliseconds
            last50ms = b50ms;

            updateUserInputs(); //update azi and elv input
            ReadEncoder();//function read the encoder

            if(cFLAG == tRun)
            {
                if (b50msec) goto eleCtrLabel; //if stay in 50 milliseconds

                offset_bang = 0.0;

                if(m_eleArray[m_index] < 1.0f)
                    offset_bang = 0;
               else
                {
                    satVel = (m_aziArray[m_index+8] - m_aziArray[m_index]); //differencs the position with respect to 8 points
                    if (satVel>180)  satVel-=360;
                    if (satVel<-180) satVel+=360;
                    satVel/=8.0f;
                    m_aziVelAnt = (m_aziArray[m_index+8] - m_aziOffset - m_ant.yy);
                    if (m_aziVelAnt >  180)  m_aziVelAnt-=360;
                    if (m_aziVelAnt < -180)  m_aziVelAnt+=360;
                    m_aziVelAnt/=8.0f;

                    if ( fabs(m_aziArray[m_index] - m_aziOffset - m_ant.yy) < 0.045 )
                        offset_bang = satVel + (m_aziArray[m_index] - m_aziOffset - m_ant.yy)/5;
                    else if((fabs(m_aziArray[m_index] - m_aziOffset - m_ant.yy) < 0.09)&& (fabs(satVel)>0.035))
                        offset_bang = satVel + (m_aziArray[m_index] - m_aziOffset - m_ant.yy)/5;
                    else
                        offset_bang = m_aziVelAnt;

                    if( (satVel>0.0 && offset_bang<0.0)||(satVel<0.0 && offset_bang>0.0) )
                    {
                        double fAbssatVel = fabs(satVel);
                        int nTime =0.0;

                        if(fAbssatVel < MaxBang1)
                            nTime = (int)((fAbssatVel/Freq_Cons_Bang-MinFreq)/.2f)+2;
                        else if(fAbssatVel< MaxBang2)
                            nTime = (int)((MaxFreqBang1-MinFreq)/.2f+(fAbssatVel/Freq_Cons_Bang - MaxFreqBang1)/.1f)+2;
                        else if(fAbssatVel< MaxBang3)
                            nTime = (int)((MaxFreqBang1 - MinFreq)/.2f+(MaxFreqBang2 - MaxFreqBang1)/.1f +
                           (fAbssatVel/Freq_Cons_Bang - MaxFreqBang2)/.05f)+2;
                        else
                            nTime = (int)((MaxFreqBang1 - MinFreq)/.2f+ (MaxFreqBang2 - MaxFreqBang1)/.1f +
                                  (MaxFreqBang3 - MaxFreqBang2)/.05f)+2;

                        if((nTime*fAbssatVel) > fabs(m_aziArray[m_index] - m_aziOffset - m_ant.yy))
                        {
                            offset_bang = m_aziVelAnt;

                            if((offset_bang >.0 && satVel <.0) || (offset_bang <.0 && satVel>.0))
                                offset_bang = satVel/fabs(satVel)*MinBang;
                        }
                        else if(fabs(m_aziArray[m_index] - m_aziOffset - m_ant.yy) <.02f)
                            offset_bang = satVel;
                }

                if((fabs(m_aziArray[m_index] - m_aziOffset - m_ant.yy) <.02f) && (fabs(satVel) < MinBang) )
                    offset_bang = 0.0f;
                }
            } else {
                offset_bang = (m_aziArray[0] - m_aziOffset - m_ant.yy);
            }

aziCtrLabel:
            ControlAzimuth();// aziMotorControl()

            if(cFLAG == tRun) {

                if (b50msec) goto eleCtrLabel;

                offset_an = 0.0;

                /*Elevation*/
                if((m_eleArray[m_index]<3.0f)||(m_eleArray[m_index]>88.7f))
                    offset_an = 0.0;
                else
                {

                    if((maxcount - m_index >2)&&(maxcount - m_index < 10))
                        satVel = ((m_eleArray[maxcount]- m_eleArray[m_index +2])/(double)(maxcount - m_index -2));
                    else
                        satVel = (m_eleArray[m_index+8] - m_eleArray[m_index]);

                    if (satVel>180)  satVel-=360;
                    if (satVel<-180) satVel+=360;
                    satVel/=8.0f;

                    m_eleVelAnt = (m_eleArray[m_index+8] - m_eleOffset - m_ant.xx);

                    if (m_eleVelAnt >  180)  m_eleVelAnt-=360;
                    if (m_eleVelAnt < -180)  m_eleVelAnt+=360;
                    m_eleVelAnt/=8.0f;

                    if ( (fabs(m_eleArray[m_index] - m_eleOffset - m_ant.xx) < 0.045))
                        offset_an = satVel + (m_eleArray[m_index] - m_eleOffset - m_ant.xx)/5;
                    else if(fabs(m_eleArray[m_index] - m_eleOffset - m_ant.xx) <.9 && fabs(satVel)>0.035)
                        offset_an = satVel + (m_eleArray[m_index] - m_eleOffset - m_ant.xx)/5;
                    else
                        offset_an = m_eleVelAnt;

                    if((satVel>.0 && offset_an<.0)||(satVel<.0 && offset_an>.0))
                    {
                        double fAbsSatVel  = fabs(satVel);
                        int nTime  = 0;

                        if(fAbsSatVel< MaxAng)
                            nTime = (int)((fAbsSatVel/Freq_Cons_Ang - MinFreq)/.6f)+2;
                        else
                            nTime = (int)((MaxFreqAng/Freq_Cons_Ang - MinFreq)/.6f)+2;

                        if((nTime*fAbsSatVel) > fabs(m_eleArray[m_index] - m_eleOffset - m_ant.xx))
                        {
                            offset_an = m_eleVelAnt;
                            if((offset_an >.0 && satVel<.0)||(offset_an <.0 && satVel>.0) )
                            offset_an = satVel/fabs(satVel)*MinAng;
                        }
                        else if(fabs(m_eleArray[m_index] - m_eleOffset - m_ant.xx) < .02)
                            offset_an = satVel;
                    }

                    if((fabs(m_eleArray[m_index] - m_eleOffset - m_ant.xx) < .02) && fabs(satVel) < MinAng)
                        offset_an = 0.0f;

                    if ( (offset_an < 0.0f) && (m_ant.xx < 6.0f) ){
                        m_eleVelAnt = 0;
                        offset_an = 0.0;
                    }

                    if ( (offset_an > 0.0f) && m_ant.xx> 87.0) {
                        offset_an = 0.0f;
                        m_eleVelAnt = 0;
                    }
                }
            } else {
                offset_an = (m_eleArray[0] - m_eleOffset - m_ant.xx);
            }

eleCtrLabel:
            ControlElevation();// eleMotorControl()
        }


        emit stepSatAnt(m_aziArray[m_index], m_eleArray[m_index],m_ant.yy,m_ant.xx); //paint:real position of the satellite
                                                                                  //and antena
        msleep(1);
    }

    delete m_aziArray;
    delete m_eleArray;

    for(int i = 0; i < 3; ++i) //for stoped the moved
    {
//        m_ddk.m_pData21 = 0x01000000;
//        m_ddk.DevIO(4);
//        m_ddk.m_pData21 = 0x000;
//        m_ddk.DevIO(4);
    }
}

void TrackerThread::updateUserInputs()
{
    if (m_userAzi){
        m_aziOffset = m_aziC*0.05;
        m_userAzi=false;
    }

    if (m_userEle){
        m_eleOffset = m_eleC*0.05;
        m_userEle=false;
    }
}

void TrackerThread::setAziOffset(int n)
{
    m_aziC=n;
    m_userAzi=true;
}

void TrackerThread::setEleOffset(int n)
{
    m_eleC=n;
    m_userEle=true;
}
//calculate de track of the satellite
void TrackerThread::precalcSatTrack(cOrbit *orbit, double endTime)
{
    const double dtDay = 0.1/SEC_PER_DAY;//count of milisegundos, one day
    const double dtMin = 0.1/60;//count the minutes which take place in 100 milisec
    const double tleEpoch = orbit->Epoch().m_Date;
    double a, tsince;
    cEci eci;
    cCoordTopo aziEle;
    cCoordGeo geoCoord;

    m_initDate = time(0);
    struct tm curr = *localtime( &m_initDate );
    curr.tm_year+=1900;
    curr.tm_mon++;
    cJulian iterTime(curr.tm_year, curr.tm_mon, curr.tm_mday,
                     curr.tm_hour, curr.tm_min, curr.tm_sec);
    iterTime.AddDay(TimeZone);
    m_nPoints = (endTime-iterTime.m_Date)/dtDay +.5;

    if (m_aziArray || m_eleArray){
        delete [] m_aziArray;
        delete [] m_eleArray;
    }
    m_aziArray = new double [m_nPoints];
    m_eleArray = new double [m_nPoints];


    FILE *fout = fopen("data.txt", "wt");
    a = (iterTime.m_Date - tleEpoch)*MIN_PER_DAY;
    for (int i=0; i<m_nPoints; ++i){
        tsince = a + i*dtMin;
        eci = orbit->GetPosition(tsince);
        aziEle = myStation.GetLookAngle(eci);
        geoCoord = eci.ToGeo();

        m_aziArray[i] = rad2deg(aziEle.m_Az);
        m_eleArray[i] = rad2deg(aziEle.m_El);

        fprintf(fout, "%f %f  %f\n", m_aziArray[i], m_eleArray[i], tsince);
    }
    fclose(fout);
   emit predictedSatTrack(m_aziArray, m_eleArray, m_nPoints);//signal emiting for painter to satellite track
}
// 0x01fff mascara de 13 bits
// firts 13 bits -> azimut encoder
// >> seconds 13 bits -> elevation encoder
void  TrackerThread::ReadEncoder()
{
//    m_ddk.DevIO(5);
//    m_ant.xx = fmod(360. + grayCode2Angle[(m_ddk.m_pData20>>13)&0x01fff] + an_off + 360.,360.);//elevation
//    m_ant.yy = fmod(720. - grayCode2Angle[m_ddk.m_pData20&0x01fff] + bang_off, 360.); //azimut
}

void TrackerThread::ControlElevation()
{
    float fTempOff=offset_an;
    float fSign=1.0;

    if(fTempOff<0.0f)
        fSign=-1.0;

    if(fAngOff0==0.)
    {
        if(fTempOff!=0.)
        {
            if(bEleControlSkip)
            {
                fTempOff=MinAng*fSign;
                bEleControlSkip=false;
            }
            else
            {
                fTempOff=.0;
                bEleControlSkip=true;
            }
        }
        else
            bEleControlSkip=true;
    }

    else

    if(fTempOff!=0.0)
    {
        bEleControlSkip=false;
        float fSign0=1.0;

        if(fAngOff0<0.0)
            fSign0=-1.0;

        if(fSign==fSign0)
        {
            float fTemp=fabs(fAngOff0)-fabs(fTempOff);

            if(fTemp>(0.5*Freq_Cons_Ang))//0.00725/**0,003625  **/
                fTempOff=(fabs(fAngOff0)-0.5*Freq_Cons_Ang)*fSign;
            else
            if(fTemp<(-0.3*Freq_Cons_Ang))//0,002175
                fTempOff=(fabs(fAngOff0)+.3*Freq_Cons_Ang)*fSign;
        }
        else
            fTempOff=0.0;
    }
    else
        bEleControlSkip=false;

    if(commandcoun<10 && fTempOff!=0.0)
    {
        fTempOff=MinAng*fSign;
    }
    commandcoun++;

//    m_ddk.m_pData21=0x00; //24bit 1-Azimuth,0-Elevation)

    int nTemp=0x00;

    if(fTempOff!=0.0)
    {
        if(fabs(fTempOff)<MinAng)
            fTempOff=MinAng*fSign;
        else
        if(fabs(fTempOff)>MaxAng)
            fTempOff=MaxAng*fSign;

        float ftempFreq=fabs(fTempOff)/Freq_Cons_Ang;

        nTemp= (int)(16500.f/ftempFreq+.5)-2;//Calculate Elevation control value from frequency

//        m_ddk.m_pData21|=(0x040000+nTemp);//19bit-Elevation Enable,nTemp-Azimuth control value (control value->control command)

        if(fTempOff>0.0)
//            m_ddk.m_pData21|=0x080000;//20bit-Elevation Direction

        fAngOff0=fSign*16500.f/(nTemp+2)*Freq_Cons_Ang;

    }
    else
        fAngOff0=0.0;

//    m_ddk.DevIO(4);
    return;
}

void TrackerThread::ControlAzimuth()
{
    float fTempOff=offset_bang;
    float fSign=1.0;

    if(fTempOff<0.0f)
        fSign=-1.0;

    if(fBangOff0==0.)
    {
        if(fTempOff!=0.)
        {
            if(bAziControlSkip)
            {
                fTempOff=MinBang*fSign;
                bAziControlSkip=false;
            }
            else
            {
                fTempOff=.0;
                bAziControlSkip=true;
            }
        }
        else
            bAziControlSkip=true;
    }
    else
    if(fTempOff!=0.0)
    {
        float fSign0=1.0;

        if(fBangOff0<0.0)
            fSign0=-1.0;

        if(fSign==fSign0)
        {
            float fTemp=fabs(fBangOff0)-fabs(fTempOff);

            if(fabs(fBangOff0)<MaxBang1)
            {
                bAziControlSkip=false;

                if(fTemp>(0.1*Freq_Cons_Bang))
                    fTempOff=(fabs(fBangOff0)-0.1*Freq_Cons_Bang)*fSign;

                else

                if(fTemp<(-0.1*Freq_Cons_Bang))
                    fTempOff=(fabs(fBangOff0)+0.1*Freq_Cons_Bang)*fSign;
            }
            else
            {
                if(fabs(fBangOff0)<MaxBang2)
                {
                    bAziControlSkip=false;

                    if(fTemp>(0.05*Freq_Cons_Bang))
                        fTempOff=(fabs(fBangOff0)-0.05*Freq_Cons_Bang)*fSign;
                    else

                    if(fTemp<(-0.05*Freq_Cons_Bang))
                        fTempOff=(fabs(fBangOff0)+0.05*Freq_Cons_Bang)*fSign;

                    int nTemp0=	(int)(16500.f/fabs(fBangOff0)*Freq_Cons_Bang+.5)-2;
                    int nTemp1=	(int)(16500.f/fabs(fTempOff)*Freq_Cons_Bang+.5)-2;

                    if((nTemp0==nTemp1) && (fabs(fTemp)>.25))
                    {
                        if(fTemp>0)
                            fTempOff=fSign*16500.f/(nTemp0+3)*Freq_Cons_Bang;
                        else
                            fTempOff=fSign*16500.f/(nTemp0+1)*Freq_Cons_Bang;
                    }
                }
                else
                if(bAziControlSkip)
                {
                    if(fTemp>(0.05*Freq_Cons_Bang))
                        fTempOff=(fabs(fBangOff0)-0.05*Freq_Cons_Bang)*fSign;
                    else
                    if(fTemp<(-0.05*Freq_Cons_Bang))
                        fTempOff=(fabs(fBangOff0)+0.05*Freq_Cons_Bang)*fSign;

                    int nTemp0=	(int)(16500.f/fabs(fBangOff0)*Freq_Cons_Bang+.5)-2;
                    int nTemp1=	(int)(16500.f/fabs(fTempOff)*Freq_Cons_Bang+.5)-2;

                    if(nTemp0==nTemp1)
                    {
                        if(fabs(fTemp)<.25 && 	(!b50msec))
                        {
                            return;
                        }
                        else

                        if(fTemp>0)
                            fTempOff=fSign*16500.f/(nTemp0+3)*Freq_Cons_Bang;
                        else
                            fTempOff=fSign*16500.f/(nTemp0+1)*Freq_Cons_Bang;
                    }
                    bAziControlSkip=false;
                }
                else
                {
                    bAziControlSkip=true;
                    return;
                }
            }

        }
        else
        {	fTempOff=0.0;bAziControlSkip=false;}
    }
    else
        bAziControlSkip=false;

//    m_ddk.m_pData21=0x01000000;//24 bit 1-Azimuth,0-Elevation)

    int nTemp=0x00;;

    if(fTempOff!=0.0)
    {
        if(fabs(fTempOff)<MinBang)
            fTempOff=MinBang*fSign;
        else
        if(fabs(fTempOff)>MaxBang3)
            fTempOff=MaxBang3*fSign;

        nTemp= (int)(16500.f/fabs(fTempOff)*Freq_Cons_Bang+.5)-2;//Calculate Azimuth control value from frequency
//        m_ddk.m_pData21|=(0x040000+nTemp);// 19bit-Azimuth Enable,nTemp-Azimuth control value (control value->control command)

        if(fTempOff>0.0)
//            m_ddk.m_pData21|=0x080000;//20bit-Azimuth Direction

        fBangOff0=fSign*16500.f/(nTemp+2)*Freq_Cons_Bang;
    }
    else
        fBangOff0=0.0;
//    m_ddk.DevIO(4);//Output the azimuth control value(control command)
    return;

}

void TrackerThread::MaxElevAnzi()
{
    maxElev = m_eleArray[0];

    for(int i =1;i < m_nPoints;i++)
    {
        if(m_eleArray[i]>maxElev)
        {
            maxElev = m_eleArray[i];
            maxAzi = m_aziArray[i];
            maxcount = i;
        }
        if(m_aziArray[i] < 0) m_aziArray[i] += 360;
    }
}

void TrackerThread::setStop()
{
    m_isRunning = false;
}

