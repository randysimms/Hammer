//===========================================================================
//
//	Module:	    UNHIDE
//
//	Description:
//			This program displays the title of the specified database.
//
//	Syntax:	unhide <dbname> [<server>]
//
//===========================================================================

#include <lncppapi.h>
#include <iostream.h>
#include <ctype.h>

#define ERR_BUF_SIZE 512

int main(int argc, char *argv[])
{
	char *				DbFilename;
	char *				DbServer=NULL;
	LNNotesSession		Session;
	LNDatabase			Db;
	LNString			DbTitle;
	LNSTATUS 			status;
	LNACL				Acl;
	LNACLEntry			Entry;

	char				flag;

	
	// Check the command line.
	if (argc < 2 || argc > 3)
	{
		cout << "\nUsage:\n\t" << argv[0] << " <filename> [<server>] \n" << endl;
		return (1);
	}

	// Get info from command line.
	DbFilename = argv[1];

	if (argc == 3)
		DbServer = argv[2];

	// Make the error handler throw all errors encountered during execution.
	LNSetThrowAllErrors(TRUE);

	try
	{
		// Initialize the C++ API
		Session.Init(argc, argv);

		// Get the specified database.
		Session.GetDatabase(DbFilename, &Db, DbServer);

		// Open sesame.
		Db.Open();

		// Get the database's title.
		DbTitle = Db.GetTitle();
		//Get the HideDesign Status
		status = Db.GetHideDesign();
		
		if (status != FALSE)
		{
			cout << "Would you like to UNHIDE the database?" << endl;
			cin >> flag;
			if (toupper(flag) == 'Y')
			{
				Db.SetHideDesign( FALSE );
				cout << "\nThe database design was UnHidden." << endl;
			}
		}
		else
		{
		//	Db.SetHideDesign( TRUE);
			cout << "The database design is UnHidden. Nothing changed." << endl;
		}

		// Display it.
		//cout << "The database title is:  " << DbTitle << endl;
		//cout << "The database design was unhidden." << endl;
		//cout << "Status is:  " << status << endl;

		
		//Add Your Name to the ACL as "Manager"
		status = Db.GetACL(&Acl);
		//Stat = Acl.GetEntries(&Entries);
		//status = Acl.CreateEntry("Randy Simms/Raleigh/IBM" , LNACLLEVEL_MANAGER , &Entry );
		if (status == 0)
		{
			cout << "Would you like to add your name as a MANAGER to the database ???" << endl;
			flag = ' ';
			cin >> flag;
			if (toupper(flag) == 'Y')
			{
				status = Acl.CreateEntry( Session.GetUserName(), LNACLLEVEL_MANAGER , &Entry );
				//status = Acl.SetDefaultAccessLevel( LNACLLEVEL_MANAGER );
				if (status == 0)
				{
					status = Acl.Save();
					if (status == 0)
						cout << "Your name was added to the ACL as a Manager."  << endl
						//cout << "Status: " << status;
				}
			}
		}
		else
			cout << "No changes were made to the ACL." << endl;
			
	}

	// Error handler.  If an error occurred, get the text of
	// the error message and display it.
	catch (LNSTATUS Lnerror)
	{
		char ErrorBuf[ERR_BUF_SIZE];
		LNGetErrorMessage(Lnerror, ErrorBuf, ERR_BUF_SIZE);
		cout << "Error:  " << ErrorBuf << endl;
	}

	// Close sesame.
	Db.Close();

	// Terminate the API.
	Session.Term();

	// Ciao, baby!
	return (0);
}


