//Project Name : Library Management System
//Data Structures Used: Hashing, Linked List, BST, Queues

#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

class BST* rack[10][5];
int amount = 1;

class Queue {
private:
    int rear;
    int front;
    int* arr;
public:
    Queue()
    {
        rear = front = -1;
        arr = new int[20];
    }
    void enQueue(int data)
    {
        if (rear == 19)
        {
            cout << "queue is full: " << endl;
        }
        else {

            rear++;
            arr[rear] = data;
        }
    }
    int deQueue()
    {
        int x;
        if (rear == front)
        {
            cout << "queue is empty: " << endl;
            return -1;
        }
        else {
            front++;
            x = arr[front];
            return x;
        }
    }
    bool isempty()
    {
        if (rear == front)
        {
            return true;
        }
        else {
            return false;
        }
    }
    void reset()
    {
        rear = front = -1;
    }
};

class Book {
public:
    string name;
    string author;
    int id, amount;
    int rack;
    Book* next;

    Book(string new_name, string new_author, int new_id, int new_rack,int new_amount) 
	{
        name = new_name;
        author = new_author;
        id = new_id;
        rack = new_rack;
        next = NULL;
        amount = new_amount;
    }
    
   void UpdateRecordFileHandlng(int ID,int amt ,string Name, string author_Name,int r);
};

class BST {
public:
    int id;
    BST* left, * right;
    Book* next;

    BST(int new_id) 
	{
        id = new_id;
        left = NULL;
        right = NULL;
        next = NULL;
    }
};

class User {
public:
    string name;
    Book* book;
    User* next;

    User(string new_name) 
	{
        name = new_name;
        next = NULL;
        book = NULL;
    }
};

class library 
{
private:
    User* start = NULL;
public:
    int generateAlphabetID(char c);
    int generateBookID(string basicString);

    void addBook(int shelve, string book_name, string author_name);
    BST* createBST(int id, BST* root);
    BST* searchBST(BST* root, int id);
    Book* searchBook(int rack_no, int shelve, string name, string author_name);

    void displayLibrary(BST* root);

    bool deleteBook(string name, string author_name);
    Book* searchPreviousBook(BST* root, string name, string author_name);
    bool searchBookcheck(BST* root, string name, string author_name);
    
    bool issueBook(string username, string bookname, string author_name);
    Book* searchBook(BST* bst, string name, string author_name);
    bool SearchingsearchBook(BST* bst, string name, int rack, int shelve);

    void displayIssuedBooks();
    bool returnBook(string user_name, string book_name);

    void generateLibraryData();
};

int library::generateAlphabetID(char c) 
{

    char alphabets[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's',
                        't', 'u', 'v', 'w', 'x', 'y', 'z' };
    c = tolower(c);

    for (int i = 0; i < 26; ++i) {
        if (c == alphabets[i])
            return ++i;
    }

    return -1;
}

int library::generateBookID(string str) 
{
    Queue q;
    q.reset();
    int value;
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == ' ')
        {
            q.enQueue(value);
            value = 0;
        }
        else {
            value += generateAlphabetID(str[i]);
        }
    }
    q.enQueue(value);
    value = 0;
    stringstream string_stream;

    while (!q.isempty())
    {//add whole queue in ss
        string_stream << q.deQueue();
    }

    // Converting combining_values to int
    string_stream >> value;
    return value;
}

BST* library::createBST(int id, BST* root) 
{
	if(root == NULL)
	{
		root = new BST(id);
		return root;
	}
	if (id > root->id )
	{
		root->right = createBST(id,root->right);
	}
	else{
		root->left = createBST(id,root->left);
	}
	return root;
}

BST* library::searchBST(BST* root, int id) 
{
    if (root == NULL) {
        return NULL;
    }
    else {
        while (root != NULL) {
            if (root->id > id) 
				root = root->left;
            else if (root->id < id) 
				root = root->right;
            else return root;
        }
        return NULL;
    }
}

Book* library::searchBook(BST* root, string name, string author_name) 
{
    int id = generateBookID(name);
    Book* head = root->next;

    while (head != NULL) {
        if (head->name == name && head->author == author_name)
            return head;
        head = head->next;
    }
    return NULL;
}

    void SaveToFile(Book* head)
    {
        fstream file;
        file.open("library.txt", ios::out | ios::app);
        Book* curr = head;
        file << curr->id << " " << curr->amount << " " << curr->name << " " << curr->author << " " << endl;
        curr = curr->next;
        file.close();
        cout << "Data saved to file successfully!" << endl;
    }
    
void library::addBook(int r, string book_name, string author_name) 
{
    /*
     * rack = 0       Maths
     * rack = 1       Computer
     * rack = 2       Physics
     * rack = 3       Islamiat
     * rack = 4       Others
     *
     * r are racks   */
     
    int id = generateBookID(book_name);
    int shelve = id % 10;
    Book* new_book = new Book(book_name, author_name, id, r,amount);

    if (rack[shelve][r] != NULL) 
	{

        // Create it root and search if any BST already exists
        BST* root = rack[shelve][r];
        BST* searched = searchBST(root, id);

        // If no BST with such ID exists
        if (searched == NULL) 
		{
            BST* new_BST = createBST(id, root);

            // Adding new_book book to BST
            new_BST->next = new_book;
            SaveToFile(root->next);
        }

        // If BST exists with same ID
        else 
		{

            // Search if book with same name exists
            Book* searched_book = searchBook(searched, book_name, author_name);

            // If same book is alredy present
            if (searched_book != NULL)
            {
				searched_book->amount++;
				
				    /*fstream file, file1;
			        file.open("library.txt", ios::in);
			        {
			            file1.open("library1.txt", ios::app | ios::in);
			            file >> id >> searched_book->amount >> book_name >> author_name >> r;
			
			            while (!file.eof())
			        		file1 << id << " " << searched_book->amount << " " << book_name << " " << author_name << " " << r << endl;
			                
							file >> id >> searched_book->amount >> book_name >> author_name >> r;
			        }
			        
			        file.close();
			        file1.close();
			        remove("library.txt");
			        rename("library1.txt", "library.txt");*/
			        	
			}
					else 
					{
			            new_book->next = searched->next;
			            searched->next = new_book;
			            SaveToFile(root->next);
			        }
			        }
    }


    // If BST does not exist at shelve array
    else 
	{
        rack[shelve][r] = new BST(id);
        rack[shelve][r]->next = new_book;
        BST* root = rack[shelve][r];
        SaveToFile(root->next);
        
         
    }
}

    //Load Data from File
    void LoadFromFile()
    {
        //using data type file handling
        int id;   //admission no
        int amount;
        string name;
        string author_name; //fathername
        int rack;//Date of birth
        fstream file;
        file.open("library.txt", ios::in);
        if (!file)
        {
            cout << "Error opening file!" << endl;
        }
        else
        {
        	library l;
            while (file >> id >> amount >> name >> author_name >> rack)
            {
                // s.Insert(aID, rNo, nameStudent, fathername, d, addrss);
                 //file >> aID >> rNo >> nameStudent >> fathername >> d >> addrss;
               l.addBook(rack,name,author_name);
            }
            cout << "Data loaded from file successfully!" << endl;
        }

        file.close();

    }

void library::displayLibrary(BST* root) 
{
	
     if (root != NULL) {
        displayLibrary(root->left);

        // Displaying book linked list
        Book* head = root->next;
        while (head != NULL) {
            cout << "Book ID = " << head->id << "\t\tAmount = " << head->amount-1 << "\t\tBook Name = " << head->name << "\t\tAuthor Name = " << head->author << endl;
            head = head->next;
        }

        displayLibrary(root->right);
    }
}



bool library::searchBookcheck(BST* root, string name, string author_name) 
{
    int id = generateBookID(name);
    Book* head = root->next;

    while (head != NULL) {
        if (head->name == name && head->author == author_name)
            return true;

        head = head->next;
    }
    return false;
}

Book* library::searchPreviousBook(BST* root, string name, string author_name) 
{
    int id = generateBookID(name);
    Book* head = root->next;
    Book* prev_book = head;

    while (head != NULL) {
        if (head->name == name && head->author == author_name)
            return prev_book;
        prev_book = head;
        head = head->next;
    }
    return NULL;
}

bool library::deleteBook(string name, string author_name) 
{
    int id = generateBookID(name);
    // For each rack
    for (int c = 0; c < 5; ++c) 
	{
        // For shelve
        int shelve = id % 10;

        // search BST
        BST* bst = searchBST(rack[shelve][c], id);

        // If BST found
        if (bst != NULL) 
		{
            	// Searching book
            if (searchBookcheck(bst, name, author_name)) 
			{
				Book* book = searchBook(bst, name, author_name);
                Book* prevbook = searchPreviousBook(bst, name, author_name);
                if (book->name == name && book->author == author_name) 
				{
                    Book* del_book = book;
                	if (book->amount > 1) 
					{
                    	book->amount--;
                	}
            
                	else if(prevbook != NULL) 
					{
                		Book* del_book = book->next;
                		if (book->next->name == name && book->next->author == author_name)
						{
               			// If amount is greater than 1
                    		if (book->next->amount > 1) 
							{
                    			book->next->amount--;
                    			return true;
                			}
                    		if (book->next->next != NULL) 
							{
                        		book->next = book->next->next;
                    		}
                    		else 
							{
                        		book->next = NULL;
                    		}
                    		delete (del_book);
                    		return true;
                		}
            		}
                	else 
					{
                    	if (book->next == NULL) 
						{
                        	bst->next = NULL;
                    	}
                    	else
						{
                        	bst->next = book->next;
                    	}
                	}
                	return true;
            	}
        	}
    	}
	}

    return false;
}

bool library::SearchingsearchBook(BST* root, string name, int rack, int shelve) 
{
    int id = generateBookID(name);
    Book* head = root->next;
    bool found = false;

    while (head != NULL) 
	{
        if (head->name == name) 
		{
            found = true;
            cout << "\nBook name : " << head->name << "       ID : " << head->id << "   Rack: " << rack + 1
                << "  Shelve: "
                << shelve + 1 << "  Author Name: " << head->author;
        }
        head = head->next;
    }
    return found;
}


bool library::issueBook(string user_name, string book_name, string author_name) 
{
    bool user_exists = NULL;
    User* u = start;
    User* user1 = NULL;
    User* user = NULL;

    while (u != NULL) 
	{
        if (u->name == user_name) 
		{
            user_exists = true;
        }
        user1 = u;
        u = u->next;
    }
    if (!user_exists)
	{
        user = new User(user_name);

        int id = generateBookID(book_name);
        int shelve = id % 10;
        Book* book = NULL;
        Book* temp_book = NULL;
        BST* bst = NULL;

        for (int i = 0; i < 5; ++i) 
		{
            bst = searchBST(rack[shelve][i], id);
            if (bst != NULL) 
			{
                book = searchBook(bst, book_name, author_name);
                if (book != NULL) 
				{
                    temp_book = new Book(book_name, author_name, id, i,amount);
                    temp_book->author = book->author;

                    // deleteing book now
                    deleteBook(book_name, author_name);

                    // Now looking if user linked list is null
                    user->book = temp_book;
                    if (start == NULL) 
					{
                        start = user;
                    }
                    else 
					{
                        user1->next = user;
                    }

                    return true;
                }
            }
        }
        cout << "Book Not Found";
    }
    else 
	{
        cout << "\n" << "One User can Issue Only One Book";
    }
    return false;
}

bool library::returnBook(string user_name, string book_name) 
{
    // searching username in list
    User* user = start;
    User* prev_user = start;
    while (user != NULL) {
        prev_user = user;
        if (user->name == user_name) break;
        user = user->next;
    }

    // If user exists
    if (user != NULL) {
        // Searching book
        Book* book = user->book;

        // If book exists
        if (book != NULL && book->name == book_name) 
		{

            // If there is only one book in user
            if (user->book == book) {
                addBook(book->rack, book_name, book->author);
                user->book = NULL;
                delete book;

                // If the user is the root
                if (user == start) {
                    if (start->next != NULL) 
					{
                        start = start->next;
                    }
                    else {
                        start = NULL;
                    }
                }
                else 
				{
                    prev_user->next = user->next;
                    delete user;
                }
                return true;
            }
        }
        else 
		{
            cout << "\nBook not Found";
        }
    }
    else 
	{
        cout << "User Doesn't Exist";
    }
    return false;

}

void library::displayIssuedBooks()
{
    User* user = start;
    while (user != NULL) 
	{
        Book* book = user->book;

        // Books
        while (book != NULL) 
		{
            cout << "\t\tUser = " << user->name << "\t\tBook Name = " << book->name << "\t\tAuthor Name = " << book->author;

            book = book->next;
        }
        user = user->next;
    }
}

int main() 
{
    library l;
    
    int choise;
    cout << endl << endl;
    cout << "\t\t\t****************************************************************" <<endl;
    cout << "\t\t\t|----------------Library Management System---------------------|" << endl;
    cout << "\t\t\t****************************************************************" << endl;
    
	cout << endl << "\t\t\tDeveloped By\n" << endl << "\t\t\tName: SAJJAD ALI" << endl << "\t\t\tSAP: 38667" << "\n\n" << "\t\t\tName: M.FASEEH" << endl << "\t\t\tSAP: 39095" << "\n\n";

    while (true) 
	{
        cout << "\n\n\n\t\t******************     M E N U     ******************\n" << endl;
        cout << "0. Exit\n";
        cout << "1. Display All Books\n";
        cout << "2. Insert a Book\n";
        cout << "3. Delete a Book\n";
        cout << "4. Search Book By Name\n";
        cout << "5. Issue Book To Member\n";
        cout << "6. Return Book\n";
        cout << "7. Display Issues Books\n";
        cout << "\nSelect an option in intigers(0 to 7) : ";

        cin >> choise;
        switch (choise) 
		{
        	case 0: 
			{
            	exit(0);
        	}
        	case 1: 
			{
            	cout << "\n\n\n\t\t*****************     D I S P L A Y     ****************\n";
           		// For each rack
           		LoadFromFile();
            	for (int c = 0; c < 5; ++c) 
				{
                	cout << "\n\n********************   R A C K - " << c + 1 << "    ********************\n" << endl;

                	// For each shelve
                	for (int r = 0; r < 10; ++r) 
					{
                    	cout << "\n+++ S H E L V E - " << r + 1 << " +++\n";
                    	l.displayLibrary(rack[r][c]);
                	}
            	}
            	break;
        	}
        	case 2: 
			{
	            string book_name;
	            string author_name;
	
	            cout << "\n\n\n\t\t*****************     I N S E R T     *****************\n";
	            cout << "0. Back\n";
	            cout << "1. Mathematics\n";
	            cout << "2. Computer\n";
	            cout << "3. Physics\n";
	            cout << "4. Islamiat\n";
	            cout << "5. Others\n";
	            cin >> choise;
	            if (choise == 0) break;
	            else if(choise > 5 || choise < 0)
	            	cout << "invalidchoice: " << endl;
				else 
				{
	                cout << "\nBook Name : ";
	
	                cin.ignore();
	                getline(cin, book_name);
	
	                cout << "\nAuthor Name : ";
	                getline(cin, author_name);
	
	                l.addBook(choise - 1, book_name, author_name);
	            }
	            break;
			}
	        case 3: 
			{
	            string name, author_name;
	            cout << "\n\n\n\t\t*****************     D E L E T E     *****************\n";
	            cout << "\nEnter Book Name : ";
	            cin.ignore();
	            getline(cin, name);
	            cout << "\nEnter Author Name : ";
	            getline(cin, author_name);
	            bool deleted = l.deleteBook(name, author_name);
	            if (deleted) cout << "\n\nSuccessfully Deleted";
	            else cout << "\n\nBook not found";
	            break;
	        }
	        case 4: {
	            string name;
	            bool found = false;
	            cout << "\n\n\n\t\t*****************     S E A R C H     *****************\n";
	            cout << "\nEnter Book Name : ";
	            cin.ignore();
	            getline(cin, name);
	
	            int id = l.generateBookID(name);
	            // For each rack
	            for (int c = 0; c < 5; ++c) 
				{
	                // For shelve
	                int shelve = id % 10;
	
	                // search BST
	                BST* bst = l.searchBST(rack[shelve][c], id);
	
	                // If BST found
	                if (bst != NULL) 
					{
	                    found = l.SearchingsearchBook(bst, name, c, shelve);
	                    break;
	                }
	            }
	            if (!found) 
				{
	                cout << "\n\nBook not found";
	            }
	            break;
	        }
	        case 5:
	        {				
	            string book_name;
	            string author_name;
	            string username;
	            cout << "\n\n\n\t\t*****************     I S S U E     *****************\n";
	            cout << "\nEnter User Name : ";
	            cin.ignore();
	            getline(cin, username);
	            cout << "\nEnter Book Name : ";
	            getline(cin, book_name);
	            cout << "\nEnter Author Name : ";
	            getline(cin, author_name);
	
	            bool issue = l.issueBook(username, book_name, author_name);
	            if (issue) cout << "\n\nBook issued successfully!\n";
	            break;
	        }
        case 6: 
		{
            string book_name;
            string username;
            cout << "\n\n\n\t\t*****************     R E T U R N     ++++++++++\n";
            cout << "\nEnter User Name : ";
            cin.ignore();
            getline(cin, username);
            cout << "\nEnter Book Name : ";
            getline(cin, book_name);

            bool issue = l.returnBook(username, book_name);
            if (issue) cout << "\n\nBook returned successfully!\n";
            break;
        }
        case 7: 
		{
            cout << "\n\n\n\t\t*****************     I S S U E D   B O O K S     *****************\n";
            l.displayIssuedBooks();
            break;
        }
        default: 
		{
            cout << "\nInvalid i nput!";
        }
        }
    }
}