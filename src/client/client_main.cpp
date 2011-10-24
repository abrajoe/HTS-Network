//Armin Mehr
//Auf Mohamed
#include <iostream>
#include <sstream>
#include "mail.h"
#include "read.h"
#include "login.h"
#include "client.h"
#include <deletemessage.h>
#include <listmessage.h>
#include <signal.h>
#include <stdio.h>

static void getrestrictedlength(std::string name, std::string& input, int size)
{
     
     std::cout<<name<<std::endl;
     std::getline(std::cin,input);
     if( input.size() > size || input.size() < 1)
     {
       input = "";
       getrestrictedlength(name,input,size);
     }
//       if(input.size() < size && input.size()>1)
//       {
// 	break;
//       }
}

int main(int argc, char **argv)
{
   if (argc == 3)
   {
      std::string name;
      std::cout << "Please enter your username:" << std::endl;
      std::getline(std::cin,name);
      Client client(Client::IPv4, Client::TCP, atoi(argv[2]), name);



      if (client.connect_to_target(std::string(argv[1])))
      {
         std::cout << "Successfully connected" << std::endl;
         signal(SIGINT, SIG_IGN);
      }

      else
      {
         std::cout << "Connection ERROR" << std::endl;
         exit(1);
      }

      /** beginn testen von login **/
      std::string user, password;
      std::cout << "Please enter your username:" << std::endl;
      std::getline(std::cin,user);
      std::cout << "Please enter your password:" << std::endl;
      std::getline(std::cin,password);

      Login lo(user,password);
      Message * lomes = &lo;
      client.sendmessage(lomes);
      client.waitresponse();

      if(client.loggedIn == true){
       std::cout << "trueeee" <<std::endl;
      }else{
        std::cout << "falseeee" << std::endl;
      }
      /** ende testen von login **/

      /*void func(int);

       signal(SIGKILL,func);*/

      std::string command;

      while (command != "EXIT")
      {
         //command = "SEND";
         std::cout << "Enter Command:" << std::endl;
         std::getline(std::cin,command);

         if (command == "SEND")
         {
            std::string from, to, subject;
            getrestrictedlength("From:(max. 8)", from, 8);
            getrestrictedlength("To: (max. 8)", to, 8);
            getrestrictedlength("Subject: (max. 80)", subject, 80);
	    
            std::cout <<"Message: (The delim char is '.')";
            char message[1024];
	    std::string mmm;
            std::cin.getline(message, 1024, '.');
	    std::cin.ignore();
            Message * m = new Mail(to, from, subject, message);
	    
            client.sendmessage(m);
	    client.waitresponse();
            delete dynamic_cast<Mail*>(m);
         }

         else if (command == "LIST")
         {
            std::string user;
            getrestrictedlength("Username:", user, 8);
            Message * mes = new Listmessage(user);
            client.sendmessage(mes);
	    client.waitresponse();
            delete dynamic_cast<Listmessage*>(mes);
         }

         else if (command == "EXIT")
         {
            client.closeconnection();
         }

         else if (command == "DEL")
         {
            int nr;
            std::string name;
	    std::string nur;
            std::cout << "Username:" << std::endl;
            std::getline(std::cin, name);
            std::cout << "NR:" << std::endl;
            std::getline(std::cin,nur);
	    nr = atoi(nur.c_str());
            Deletemessage dm(nr, name);
            Message * mes = &dm;
            client.sendmessage(mes);
	    client.waitresponse();
         }

         else if (command == "READ")
         {
            std::string user;
            std::string number;
            getrestrictedlength("Username:", user, 8);
            std::cout << "Messagenumber: ";
            std::getline(std::cin, number);
            Message * mes = new Read(user, atoi(number.c_str()));
            client.sendmessage(mes);
            client.waitresponse();
	    delete dynamic_cast<Read*>(mes);
         }
      }


      /*
       Message * mg = new Mail();
       dynamic_cast<Mail*>(mg)->_msg ="hiiii";
       std::stringstream oss;
       boost::archive::text_oarchive oa(oss);
       oa << mg;

       Message * msg;
       boost::archive::text_iarchive ia(oss);
       ia >> msg;

       switch(msg->getmessagetype())
       {
       case Message::mMail:
       std::cout<< dynamic_cast<Mail*>(msg)->_msg<<std::endl;
       break;
       /*case LIST
       * case exit
       * ....usw

       }*/
   }

   else
   {
      std::cout << "Wrong usage" << std::endl << "Usage:" << std::endl
                << argv[0] << " <IP> <PORT>" << std::endl;
   }
}
