#include<iostream>
#include<fstream>    // For File-operations
#include<string.h>   // For String manipulation
#include<conio.h>    // For getch() func.
#include<ctime>      // For current-Time
using  namespace std;

typedef struct {                                           // User Data outline
	int posts,id;
	char name[40],pass[30],mail_id[30];
}Data;

typedef struct {                                          //  Blog Post outline
	char date[15],time[10],post_by[40],title[90];
}Post;

void init_files()	{                                     //  Checking and Creating necessary Folders & Files...
	int flag=0;
	fstream file;
	file.open("Users\\Profiles.dat",ios::in);
	if(!file){
		system("mkdir Users");
		system("mkdir Posts");
		flag=1;
	}
	file.close();
	if(flag)	{
		file.open("Users\\Profiles.dat",ios::out);
		file.close();
		file.open("Users\\user_count.txt",ios::out);
		file<<0;
		file.close();
	}
}

void get_pass(char *str)	{               //  HTML_Style Password receiver
	time_t begin,end;
	int pos=0;
	float t;
	char let;
	while(true)	{
		let=getch();
		if(let==13)	break;
		if(let==8)	{
			if(pos<=0)	continue;
			cout<<"\b \b";
			str[--pos]='\0';
		}
		else	{
			if(pos<0) pos=0;
			str[pos++]=let;
			begin=clock();
			t=0;
			cout<<let;
			while(t<0.2)	{
				end=clock();
				t=(float)(end-begin)/CLOCKS_PER_SEC;
			}
			cout<<"\b*";
	    }  
	}
	str[pos]='\0';
}

void get_time(char *d,char *t)	{          //  Current system-time getter
	int j=0,k=0;
	char str[20];
	time_t now;
	struct tm *ti;
	time(&now);
	ti = localtime(&now);
	strcpy(str,asctime(ti));
	for(int i=4;i<strlen(str);i++)	{
		if(i<=10||i>19) d[j++]=str[i];
		else	t[k++]=str[i];	
	}
	d[--j]='\0';
	t[--k]='\0';
}

class User	{
	public:
	void view_posts()	{
		Post temp;
		char opt,skip,blg[125];
		int flag;
		fstream file,fout;
		file.open("Posts\\Posts.txt",ios::in);
		if(!file)	{
			cout<<"\n\nNo Blog Posts made yet :( !\n";
			return;
		}
		else	{
			fout.open("Posts\\Temp.txt",ios::app);
			do{
				system("cls");                           // Read data in format written
				file>>ws;
				file.getline(temp.post_by,40,'\n');
				file.getline(temp.date,15,'\n');
				file.getline(temp.time,10,'\n');
				file.getline(temp.title,90,'\n');
				if(file.eof())	{
					cout<<"\nOops ! We've ran out of posts.\n\nCome back later for more posts...\n\n";
					break;
				}
				fout<<"\n"<<temp.post_by<<"\n";         //  Print out the data
				fout<<temp.date<<"\n";
				fout<<temp.time<<"\n";
				fout<<temp.title<<"\n";
				cout<<"\nPosted on : "<<temp.date<<" , "<<temp.time;
				cout<<"\nPosted by : "<<temp.post_by;
				cout<<"\n\n"<<temp.title<<" :-";
				cout<<"\n\n";
				do{
					file.getline(blg,100,'\n');          //  Ouput line-by-line blog content
					if(strlen(blg)==0) continue;
					fout<<blg<<"\n";
					cout<<blg<<"\n";
				}while(strlen(blg)!=0);
				fout<<"\n";
				cout<<"\n\n1.Next Post\n2.Back\n\nEnter your option : ";
				opt=getche();
				switch(opt)	{
					case '1':
						continue;
					case '2':
						flag=1;
						goto end;
					default :
						cout<<"\n\nEnter a Valid option !\n";
				}
			} while(!file.eof());
			end:
			if(flag)	{             // When user wants to exit half-way, copy the rest of file content to temp file.
				do{
					file>>ws;
					file>>temp.post_by;
					file>>temp.date;
					file>>temp.time;
					file>>temp.title;
					if(file.eof()) break;
					fout<<"\n"<<temp.post_by<<"\n";
					fout<<temp.date<<"\n";
					fout<<temp.time<<"\n";
					fout<<temp.title<<"\n";
					do{
						file.getline(blg,100,'\n');
						if(strlen(blg)==0) continue;
						fout<<blg<<"\n";
					}while(strlen(blg)!=0);
				}while(!file.eof());
			}
		}
		file.close();
		fout.close();
		system("del Posts\\Posts.txt");
		system("ren Posts\\Temp.txt Posts.txt");
	}
	
	void view_users()	{
		Data user;
		int flag=0;
		char nam[40];
		fstream file;
		file.open("Users\\Profiles.dat",ios::in);
		cout<<"\nWelcome to User Search Portal...\n\nEnter Username : ";
		scanf(" %[^\n]s",nam);
		if(file)	{
			do{
				file.read((char *) &user,sizeof(user));        // Search User by name
				if(file.eof()) break;
				if(strcmp(user.name,nam)==0)	{
					flag=1;
					cout<<"\nAccount Found !\n";
					break;
				}
			}while(!file.eof());
		}
		file.close();
		if(flag)	{
			cout<<"\nUsername    : "<<user.name<<"\nMail_ID     : "<<user.mail_id<<"\nUser_ID     : "<<user.id<<"\nTotal Posts : "<<user.posts<<"\n\n";
		}
		else	{
			cout<<"\nUsername does not exist !";
		}
	}
	virtual void sign_up(){};
	virtual int login(){};
	virtual void greet(){};
	virtual void make_post(){};
};

class newUser : public User	{
	Data user;
	public:
	newUser()	{
		user.posts=0,user.id=0;
	}
	
	void sign_up()	{
		int flag,count;
		Data temp;
		fstream file,cf;
		cout<<"\nWelcome to BLOGSTAGRAM, Newbie !\nStart up by creating your Account here....\n";
		do	{
			flag=1;
			char name[40];
			cout<<"\nEnter Username : ";
			scanf(" %[^\n]s",user.name);
			file.open("Users\\Profiles.dat",ios::in | ios::binary);
			if(file)	{
				do{
					file.read((char *) &temp,sizeof(temp));
					if(file.eof()) break;
					if(strcmp(user.name,temp.name)==0)	{             // Check if username is already present
						flag=0;
						cout<<"\nUsername already exists! Use other name...\n";
						break;
					}
				}while(!file.eof());
			}
			file.close();
			if(!flag) continue;                // Continue till user enters a new name
			cout<<"\nEnter Password : ";
			get_pass(user.pass);
			cout<<"\n\nEnter Mail_ID  : ";
			scanf(" %[^\n]s",user.mail_id);
			
			cf.open("Users\\user_count.txt",ios::in);
			cf>>count;
			user.id=507701+count;
			cf.close();
			
			cf.open("Users\\user_count.txt",ios::out);        // Storing no.of users to generate user-id for new user
			cf<<count+1;
			cf.close();
			
			file.open("Users\\Profiles.dat",ios::app | ios::binary);
			file.write((char*) &user,sizeof(user));
			file.close();
			
			system("cls");
			cout<<"\nAccount created Successfully...!";                       // Display entered details for reference
			this->disp_creds();
			cout<<"These credentials are essential to Login later...\n\n";
		}while(!flag);
	}
	
	void disp_creds()	{
		cout<<"\n\nYOUR DETAILS :-\n\n";
		cout<<"Username    : "<<user.name<<"\nPassword    : "<<user.pass<<"\nMail_ID     : "<<user.mail_id<<"\nUser_ID     : "<<user.id<<"\nTotal Posts : "<<user.posts<<"\n\n";
	}
};

class oldUser : public User	{
	Data user;
	public:
	
	int login()	{
		fstream file;
		int flag=0;
		char nam[40],pas[25];
		file.open("Users\\Profiles.dat",ios::in | ios::binary);
		
		cout<<"\nEnter Username : ";
		scanf(" %[^\n]s",nam);
		if(file)	{
			do{
				file.read((char *) &user,sizeof(user));    // Check for user-name in profiles present
				if(file.eof()) break;
				if(strcmp(user.name,nam)==0)	{
					flag=1;
					break;
				}
			}while(!file.eof());
		}
		file.close();
		if(flag)	{                                   //  Only if name exists...
			cout<<"\nEnter Password : ";
			get_pass(pas);
			if(strcmp(user.pass,pas)==0)	return 1;
			cout<<"\n\nIncorrect Password !\n";
		}
		else	cout<<"\nThis Username does not exist...!\n";
		return 0;
	}
	
	void make_post()	{
		Post temp;
		Data tmp;
		fstream file,fpro,ftem;
		char blg[100];
		file.open("Posts\\Posts.txt",ios::app);
		fpro.open("Users\\Profiles.dat",ios::in|ios::binary);
		ftem.open("Users\\Temp.dat",ios::app|ios::binary);
		cout<<"\nEnter Blog Header / Title  : ";
		scanf(" %[^\n]s",temp.title);
		strcpy(temp.post_by,user.name);
		get_time(temp.date,temp.time);
		file<<"\n"<<temp.post_by<<"\n";			// Writing current time & user's data to file
		file<<temp.date<<"\n";
		file<<temp.time<<"\n";
		file<<temp.title<<"\n";
		cout<<"\n( Tip : To finish the blog press 'Enter' key twice. )\n\nStart Writing your Blog Content Down Here....\n\n";
		cin >> ws;
		do{
			cin.getline(blg,100);               // get & write line-by-line to file
			if(strlen(blg)==0) continue;
			file<<blg<<"\n";
		}while(strlen(blg)!=0);
		file<<"\n";
		file.close();
		if(fpro)	{
			do{
				fpro.read((char *) &tmp,sizeof(tmp));
				if(file.eof()) break;
				if(strcmp(tmp.name,user.name)==0)	++tmp.posts;     // Increment no.of posts by current user.
				ftem.write((char*) &tmp,sizeof(tmp));
			}while(!fpro.eof());
		}
		fpro.close();
		ftem.close();
		system("del Users\\Profiles.dat");
		system("ren Users\\Temp.dat Profiles.dat");
		cout<<"\nBlog Created & Posted successfully...!\n";
	}
	
	void greet()	{cout<<"\nWelcome back, "<<user.name<<" !";}
};

main()	{
	int val,i;
	char opt1,opt2,skip;
	User *usr;                              // Super-Class pointer to achieve run-time polymorphism
	init_files();
	
	menu:
	do{
		system("cls");
		cout<<"\n\t- - - B.L.O.G. S.T.A.G.R.A.M - - -\n\n1.New User\n2.Old User\n3.Exit\n\nEnter your option : ";
		opt1=getche();
		system("cls");
		switch(opt1)	{
			case '1' :
				usr = new newUser;
				do{
					system("cls");
					cout<<"\n\n1.Sign Up\n2.View Posts\n3.View Users\n4.Back\n\nEnter your option : ";
					opt2=getche();
					system("cls");
					switch(opt2)	{
						case '1':
							usr->sign_up();
							break;
						case '2':
							usr->view_posts();
							break;
						case '3':
							usr->view_users();
							break;
						case '4':
							goto menu;
						default :
							cout<<"\n\nEnter a Valid option !\n";
					}
					cout<<"\n\nPress Any Key to Continue....";
					skip=getche();
				}while(opt2!='4');
				delete usr;
				break;
			case '2' :
				usr = new oldUser;
				if(usr->login())	{
					do{
						system("cls");
						usr->greet();
						cout<<"\n\n1.Create Post\n2.View Posts\n3.View Users\n4.Back\n\nEnter your option : ";
						opt2=getche();
						system("cls");
						switch(opt2)	{
							case '1':
								usr->make_post();
								break;
							case '2':
								usr->view_posts();
								break;
							case '3':
								usr->view_users();
								break;
							case '4':
								goto menu;
							default :
								cout<<"\n\nEnter a Valid option !\n";
						}
						cout<<"\n\nPress Any Key to Continue....";
						skip=getche();
					}while(opt2!='4');
				}
				delete usr;
				break;
			case '3':
				cout<<"\n\nGood Bye ! Thanks for using Blogstagram !\n";
				exit(0);
			default :
				cout<<"\n\nEnter a Valid option !\n";
		}
		cout<<"\nPress Any Key to Continue....";
		skip=getche();
	}while(opt1!='3');
}
