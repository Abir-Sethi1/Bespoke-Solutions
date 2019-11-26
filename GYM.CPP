#include<fstream.h>
#include<stdio.h>
#include<conio.h>
#include<string.h>
int check_purchaser();
void get_acc();
void line(char ch='*', int n=20)
{       cout<<endl;
	for(int i=0;i<n;i++)
	cout<<ch;
}
class item
{
	int code;
	char type[20], brand[20];
	float price;
	char seller_name[30];
	public:
	void getItem()
	{
		cout<<"\n Enter Details :";
		cout<<"\n Code : ";
		cin>>code;
		cout<<"\n Type : ";
		gets(type);
		cout<<"\n Price : ";
		cin>>price;
		cout<<"\n Brand : ";
		gets(brand);
		cout<<"\n Seller name : ";
		gets(seller_name);
	}
	void showItem()
	{
		cout<<"\n\n Details  \t ";
		cout<<"\n Seller   \t: "<<seller_name;
		cout<<"\n Code     \t: "<<code;
		cout<<"\n Type     \t: "<<type;
		cout<<"\n Price    \t: "<<price;
		cout<<"\n Brand    \t: "<<brand;
	}
	int Rcode()
	{
	return code;
	}
	char* Rseller()
	{
	return seller_name;
	}
	float Rprice()
	{
	return price;
	}
	char* Rbrand()
	{
	return brand;
	}
	char* Rtype()
	{
	return type;
	}
};
class account
{
	int money;
	int total_items;
	char name[30];
	char password[30];
	char ch;
	public:
	void enter()
	{
		cout<<"\n Enter account details\n";
		cout<<"\n Enter account name : ";
		gets(name);
		cout<<"\n Enter account password : ";
		gets(password);
		cout<<"\n Enter amount of money in account : ";
		cin>>money;
	}
	int Rmoney()
	{
	return money;
	}
	char* Rpass()
	{
	return password;
	}
	char* Rname()
	{
	return name;
	}

	int check_money(int amt)
	{
		if(money>=amt)
		return 1;
		else
		return 0;
	}
	void update_bought(int amt)
	{
		money=money-amt;
	}
	void update_sold(int amt)
	{
		money=money+amt;
	}

};

void sell()
{
	fstream f;
	item ob;
	char ch,ch2;
	cout<<"\n Do you have an account?(y/n)\n";
	ch2=getche();
	if(ch2!='y')
	{
		get_acc();
	}
	else
	{
		if(check_purchaser()==1)
		{
			f.open("item.dat",ios::app|ios::binary);
			do
			{
				ob.getItem();
				f.write((char*)&ob, sizeof(ob));
				cout<<"\n Write more? (Y/N)\n";
				ch=getche();
			}while(ch=='Y'||ch=='y');
		}
		f.close();
	}
}
void dispall_item(char fn[]="item.dat")
{
	fstream f;
	item ob;
	int n=0;
	f.open(fn,ios::in|ios::binary);
	while(f.read((char *)&ob,sizeof(ob)))
	{
		ob.showItem();
		n++;

	}
	f.close();
	cout<<endl<<n<<" Items found";
}
void update()
{
	fstream f;
	item ob;
	int code,found=0,qt;
	cout<<"\n Enter item code : ";
	cin>>code;
	f.open("item.dat",ios::in|ios::out|ios::binary);
       while(f.read((char*)&ob, sizeof(ob)))
       {	if(code==ob.Rcode())
		{
			ob.showItem();
			cout<<"\n Enter new details ";
			ob.getItem();
			int pos =f.tellp()-sizeof(ob);
			f.seekg(pos);
			f.write((char*)&ob, sizeof(ob));
			found++;
		}
       }
	f.close();
	cout<<found<<" Records updated";
}
void get_acc()
{
	account ob;
	fstream f;
	f.open("acc.dat",ios::out|ios::binary);
	ob.enter();
	f.write((char*)&ob,sizeof(ob));
	f.close();
}

int check_purchaser()
{
	account ob;
	int flag=0;
	char ch,name[30],pass[30];
	fstream f;
	cout<<"\n Do you have an account?(y/n)\n";
	ch=getche();
	if(ch=='n')
       {
		get_acc();
		return 0;
       }
	else
       {
		f.open("acc.dat",ios::in|ios::binary);
		cout<<"\n Enter account name :";
		gets(name);
		while(f.read((char*)&ob,sizeof(ob)))
		{
			if(strcmpi(name,ob.Rname())==0)
			{
				flag=1;
				cout<<"\n Enter password :";
				gets(pass);
				if(strcmpi(pass,ob.Rpass())==0)
				return 1;
			else
			{
				cout<<"\n Invalid password";
				return 0;
			}
			}
		}
		if(flag==0)
		{
			cout<<"\n Invalid account name";
			return 0;
		}
	}
}
void update_bought(int code,char name[])
{
	account a;
	item ob;
	fstream f,f2,f3;
	f3.open("temp.dat",ios::out|ios::binary);
	f.open("item.dat",ios::in|ios::out|ios::binary);
	f2.open("acc.dat",ios::in|ios::out|ios::binary);
	while(f.read((char*)&ob,sizeof(ob)))
	{
		if(code==ob.Rcode())
		{
			while(f2.read((char*)&a,sizeof(a)))
			{
				if(strcmpi(name,a.Rname())==0)
				{
					a.update_sold(ob.Rprice());
				}
				f3.write((char*)&a,sizeof(a));
			}
		}
	}
	f3.close();
	f.close();
	f2.close();
	remove("acc.dat");
	rename("temp.dat","acc.dat");
}
void check_money()
{
	fstream f;
	char name[30];
	account a;
	cout<<"\n Enter account name : ";
	gets(name);
	f.open("acc.dat",ios::in|ios::binary);
	while(f.read((char*)&a,sizeof(a)))
	{
		if(strcmpi(name,a.Rname())==0)
		cout<<"\n Money in account : "<<a.Rmoney();
	}
	f.close();
}
void purchase()
{
	fstream f,f1,f2,f3;
	account a;
	item ob;
	int code;
	char name[30];
	if(check_purchaser()==1)
	{
		f.open("acc.dat",ios::in|ios::binary);
		cout<<"\n Enter name again :";
		gets(name);
		while(f.read((char*)&a,sizeof(a)))
		{
			if(strcmpi(name,a.Rname())==0)
			{
				cout<<"\n Enter Item code : ";
				cin>>code;
				f3.open("temp.dat",ios::out|ios::binary);
				f1.open("item.dat",ios::in|ios::binary);
				f2.open("sold.dat",ios::app|ios::binary);
				while(f1.read((char*)&ob, sizeof(ob)))
				{	if(code==ob.Rcode())
					{
						cout<<"\n Details\n";
						ob.showItem();
						if(a.check_money(ob.Rprice())==1)
						{
							a.update_bought(ob.Rprice());
							f2.write((char*)&ob, sizeof(ob));
							update_bought(code,name);
							break;
						}
						else
						cout<<"\n Lack of funds ";
					}
					else
						f3.write((char*)&ob, sizeof(ob));
					f1.close();
					f3.close();
					f2.close();
					remove("item.dat");
					rename("temp.dat", "item.dat");
				}
				break;
			}
	       }
       }
}
void del_item()
{
	fstream f1,f2;
	item ob;
	int code,found=0;
	cout<<"\n Enter code : ";
	cin>>code;
	f2.open("temp.dat",ios::out|ios::binary);
	f1.open("item.dat",ios::in|ios::binary);
       while(f1.read((char*)&ob, sizeof(ob)))
       {	if(code==ob.Rcode())
		{
			cout<<"\n Details\n";
			ob.showItem();
			found++;

		}
		else
			f2.write((char*)&ob, sizeof(ob));
       }
	f1.close();
	f2.close();
	remove("item.dat");
	rename("temp.dat", "item.dat");
	cout<<endl<<found<<" Record deleted";
}
void search_type()
{
	fstream f;
	item ob;
	char ch[30];
	cout<<"\n Enter item type : ";
	gets(ch);
	f.open("item.dat",ios::in|ios::binary);
	while(f.read((char*)&ob,sizeof(ob)))
	{
		if(strcmpi(ch,ob.Rtype())==0)
			ob.showItem();
	}
	f.close();
}
void search_brand()
{
	fstream f;
	item ob;
	char ch[30];
	cout<<"\n Enter item brand : ";
	gets(ch);
	f.open("item.dat",ios::in|ios::binary);
	while(f.read((char*)&ob,sizeof(ob)))
	{
		if(strcmpi(ch,ob.Rbrand())==0)
		ob.showItem();
	}
	f.close();
}
void search_code()
{
	fstream f;
	item ob;
	int code,found=0,qt;
	cout<<"\n Enter item code : ";
	cin>>code;
	f.open("item.dat",ios::in|ios::out|ios::binary);
       while(f.read((char*)&ob, sizeof(ob)))
       {	if(code==ob.Rcode())
		{
			ob.showItem();
			found++;
		}
       }
	f.close();
	cout<<found<<" Records found";
}

void copy(char fn1[],char fn2[])
{
	fstream f1,f2;
	item ob;
	f1.open(fn1,ios::in|ios::binary);
	f2.open(fn2,ios::out|ios::binary);
	while(f1.read((char*)&ob,sizeof(ob)))
	{
		f2.write((char*)&ob,sizeof(ob));
	}
	f1.close();
	f2.close();
}
void search_high_to_low()
{
	int i,j,n=0;
	item ob1,ob2;
	fstream f;
	copy("item.dat","asc.dat");
	f.open("asc.dat",ios::in|ios::out|ios::binary);
	f.seekg(0,ios::end);
	n=f.tellg()/sizeof(item);
	f.seekg(0);
	for(i=0;i<n-1;i++)
	{
		for(j=0;j<n-1-i;j++)
		{
			f.seekg(j*sizeof(ob1));
			f.read((char*)&ob1,sizeof(ob1));
			f.read((char*)&ob2,sizeof(ob2));
			if(ob1.Rprice()<ob2.Rprice())
			{
				f.seekg(j*sizeof(item));
				f.write((char*)&ob2,sizeof(ob2));
				f.write((char*)&ob1,sizeof(ob1));
			}
		}
	}
	f.seekg(0);
	f.close();
	dispall_item("asc.dat");
}
void search_low_to_high()
{
	int i,j,n=0;
	item ob1,ob2;
	fstream f;
	copy("item.dat","asc.dat");
	f.open("asc.dat",ios::in|ios::out|ios::binary);
	f.seekg(0,ios::end);
	n=f.tellg()/sizeof(item);
	f.seekg(0);
	for(i=0;i<n-1;i++)
	{
		for(j=0;j<n-1-i;j++)
		{
			f.seekg(j*sizeof(ob1));
			f.read((char*)&ob1,sizeof(ob1));
			f.read((char*)&ob2,sizeof(ob2));
			if(ob1.Rprice()>ob2.Rprice())
			{
				f.seekg(j*sizeof(item));
				f.write((char*)&ob2,sizeof(ob2));
				f.write((char*)&ob1,sizeof(ob1));
			}
		}
	}
	f.seekg(0);
	f.close();
	dispall_item("asc.dat");
}

void search_menu()
{       clrscr();
	char ch;
	int code;
	do
	{
		cout<<"\n Search Menu";
		line();
		cout<<"\n1 Code\n2 Type \n3 Brand";
		cout<<"\n4 Price high to low\n5 Price low to high\n0 Exit to main";
		cout<<"\nInput your choice\n";
		ch=getche();
		switch(ch)
		{
			case '1': search_code(); break;
			case'2': search_type(); break;
			case '3': search_brand();break;
			case'4': search_high_to_low();break;
			case'5': search_low_to_high();break;

		}
	}while(ch!='0');
}
void main_menu()
{
	clrscr();
	char ch;
	int code;
	do
	{
		cout<<"\n Main Menu";
		cout<<"\n1 Create account";
		cout<<"\n2 Sell Item\n3 Purchase item \n4 Display all items";
		cout<<"\n5 Update item\n6 Delete\n7 Search menu\n8 Check money\n0 Exit";
		cout<<"\nInput your choice\n";
		ch=getche();
		switch(ch)
		{
			case'1': get_acc();break;
			case '2': sell(); break;
			case'3': purchase(); break;
			case '4': dispall_item();break;
			case'5': update();break;
			case'6': del_item();break;
			case'7': search_menu();break;
			case'8': check_money();break;
		}
	}while(ch!='0');
}
void main()
{
	main_menu();
}