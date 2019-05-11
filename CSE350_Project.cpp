#include<iostream>
#include<cstring>
#include<cstdlib>
#include<ctime>
using namespace std;

int Timeout=3;
bool CRCFlag=false,ReceivedFlag=false,InputFlag=false;

string CRC(string Dividend,string Divisor);
string Transfer(string Send);

int main()
{
    srand (time(NULL));
    string Divisor,Data,FCS,Frame,SentFrame,ReceivedFrame;
    char o;
    while(1)
    {
        cout<<"1.\tInput Packet Data\n";
        cout<<"2.\tCompute CRC\n";
        cout<<"3.\tTransfer\n";
        cout<<"4.\tVerify CRC\n";
        cout<<"5.\tChange a Random Bit\n";
        cout<<"6.\tSet Timeout\n";
        cout<<"7.\tExit\n";
        cout<<"Enter Option: ";
        cin>>o;
        switch(o)
        {
        case '1':
        {
            cout<<"Enter Data: ";
            cin>>Data;
            cout<<"Enter Divisor: ";
            cin>>Divisor;
            bool wrong = false;
            for(int i=0; i<Data.size();i++)
                if(Data[i]!='0' && Data[i]!='1')
                    wrong = true;
            if(wrong){
                cout<<"Entered Data is Not Binary"<<endl;
                return 0;
                break;
            }
            for(int i=0; i<Divisor.size();i++)
                if(Divisor[i]!='0' && Divisor[i]!='1')
                    wrong = true;
            if(wrong){
                cout<<"Entered Divisor is Not Binary"<<endl;
                return 0;
                break;
            }
            while(Data.size()<Divisor.size())
                Data.insert(0,"0");
            cout<<"Data Inputed Successfully"<<endl;
            InputFlag=true;
            break;
        }
        case '2':
        {
            if(!InputFlag)
            {
                cout<<"Haven't Inputed Data"<<endl;
                break;
            }
            FCS ="";
            while(FCS.size()<Divisor.size()-1)
                FCS.insert(0,"0");
            Frame = Data+FCS;
            cout<<"Frame: "<<Frame<<endl;
            FCS = CRC(Frame,Divisor);
            cout<<"FCS: "<<FCS<<endl;
            Frame = Data+FCS;
            cout<<"Frame: "<<Frame<<endl;
            SentFrame = Frame;
            CRCFlag = true;
            break;
        }
        case '3':
        {
            if(!InputFlag)
            {
                cout<<"Haven't Inputed Data"<<endl;
                break;
            }
            if(!CRCFlag)
            {
                cout<<"Haven't Computed CRC"<<endl;
                break;
            }
            cout<<"Sending Frame: "<<SentFrame<<endl;
            ReceivedFrame = Transfer(SentFrame);
            cout<<"Received Frame: "<<ReceivedFrame<<endl;
            ReceivedFlag = true;
            break;
        }
        case '4':
        {
            if(!InputFlag)
            {
                cout<<"Haven't Inputed Data"<<endl;
                break;
            }
            if(!CRCFlag)
            {
                cout<<"Haven't Computed CRC"<<endl;
                break;
            }
            if(!ReceivedFlag)
            {
                cout<<"Haven't Received Data"<<endl;
                break;
            }
            Frame = ReceivedFrame;
            cout<<"Frame: "<<Frame<<endl;
            FCS = CRC(Frame,Divisor);
            cout<<"FCS: "<<FCS<<endl;
            int Flag=0;
            for(int i=0; i<FCS.size(); i++)
                if(FCS[i]=='1')
                {
                    Flag++;
                    break;
                }
            if(Flag==0)
                cout<<"CRC Verification Successful"<<endl;
            else
                cout<<"CRC Verification Failed"<<endl;
            break;
        }
        case '5':
        {
            if(!InputFlag)
            {
                cout<<"Haven't Inputed Data"<<endl;
                break;
            }
            if(!CRCFlag)
            {
                cout<<"Haven't Computed CRC"<<endl;
                break;
            }
            if(!ReceivedFlag)
            {
                cout<<"Haven't Received Data"<<endl;
                break;
            }
            cout<<"Received Frame: "<<endl;
            cout<<ReceivedFrame<<endl;
            int r = rand()%ReceivedFrame.size();
            if(ReceivedFrame[r]=='1')
                ReceivedFrame[r]='0';
            else
                ReceivedFrame[r]='1';
            for(int i=0; i<r; i++){
                cout<<" ";
            }
            cout<<"^"<<endl;
            cout<<ReceivedFrame<<endl;
            cout<<"Random Bit Changed"<<endl;
            break;
        }
        case '6':
        {
            cout<<"Enter Timeout: ";
            cin>>Timeout;
            cout<<"Timeout Set to "<<Timeout<<"s"<<endl;
            break;
        }
        case '7':
        {
            return 0;
            break;
        }
        default:
        {
            cout<<"Wrong Input"<<endl;
            break;
        }
        }
        cout<<endl;
    }
    return 0;
}

string CRC(string Dividend,string Divisor)
{
    string Temp="",Reminder;
    int FCSLen = Divisor.size()-1;

    cout<<"Length: "<<Dividend.size()<<endl;

    for(int i=0; i<Dividend.size(); i++)
    {
        if(Temp.size()<Divisor.size())
            Temp.push_back(Dividend[i]);

        cout<<"i: "<<i+1<<"\t"<<"Temp: "<<Temp<<"\t";

        if(Temp.size()==Divisor.size())
        {
            Reminder="";
            for(int i=0; i<Temp.size(); i++)
                if(Temp[i] == Divisor[i])
                    Reminder.push_back('0');
                else
                    Reminder.push_back('1');
            while(Reminder[0]=='0')
                Reminder.erase(0,1);
            cout<<"Subtract Result: "<<Reminder;
            Temp = Reminder;
        }
        cout<<endl;
    }
    while(Temp.size()<FCSLen)
        Temp.insert(0,"0");
    return Temp;
}

string Transfer(string Send)
{
    string Receive = "";
    char o = '9';
    int i=0,WT=0,Time = 1;
    cout<<"Length: "<<Send.size()<<endl;
    while(1)
    {
        cout<<"Time: "<<Time<<"s"<<endl;

        if(Time==1)
        {
            WT=0;

            Receive.push_back(Send[i]);

            cout<<"Sender --------->\t";
            cout<<"Data["<<i<<"]";
            cout<<" \t---------> Receiver"<<endl;
        }

        if(o=='1')
        {
            i++;
            if(Send.size()-1==i){
                cout<<"Sender <---------\t";
                cout<<"ACK["<<i<<"]";
                cout<<"  \t<--------- Receiver"<<endl;

                Receive.push_back(Send[i]);

                cout<<"Sender --------->\t";
                cout<<"  EOT";
                cout<<" \t\t---------> Receiver"<<endl;
                return Receive;
            }
            WT=0;
            cout<<"Sender <---------\t";
            cout<<"ACK["<<i<<"]";
            cout<<"  \t<--------- Receiver"<<endl;

            Receive.push_back(Send[i]);

            cout<<"Sender --------->\t";
            cout<<"Data["<<i<<"]";
            cout<<" \t---------> Receiver"<<endl;
        }
        else if(o=='2')
        {
            WT=0;
            cout<<"Sender <---------\t";
            cout<<"  NAK";
            cout<<"  \t\t<--------- Receiver"<<endl;

            Receive.erase(Receive.size()-1,1);
            Receive.push_back(Send[i]);

            cout<<"Sender --------->\t";
            cout<<"Data["<<i<<"]";
            cout<<" \t---------> Receiver"<<endl;
        }
        else if(o=='0')
        {
            WT++;
            cout<<"Wait Time: "<<WT<<"s"<<endl;
        }
        if(WT==Timeout)
        {
            WT=0;
            cout<<"Timeout ("<<Timeout<<"s)"<<endl;

            Receive.erase(Receive.size()-1,1);
            Receive.push_back(Send[i]);

            cout<<"Sender --------->\t";
            cout<<"Data["<<i<<"]";
            cout<<" \t---------> Receiver"<<endl;
        }
        cout<<"Enter 0 to Do Nothing\t\t(Lost Frame / Retransmission)"<<endl;
        cout<<"Enter 1 to Send ACK["<<i<<"]\t\t(Positive Acknowledgment)"<<endl;
        cout<<"Enter 2 to Send NAK\t\t(Negative Acknowledgment)"<<endl;
        cin>>o;
        if(o=='0' || o=='1' || o=='2')
            Time++;
        else
            cout<<"Invalid Input"<<endl;
        cout<<endl;
    }
}

