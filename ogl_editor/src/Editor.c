#include <string.h>
#include <stdlib.h>
#include <Emgui.h>
#include <stdio.h>
#include "Dialog.h"
#include "Editor.h"
#include "LoadSave.h"
#include "TrackView.h"
#include "rlog.h"
#include "RemoteConnection.h"
#include "../../sync/sync.h"
#include "../../sync/base.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(EMGUI_MACOSX)
#define FONT_PATH "/Library/Fonts/"
#elif defined(EMGUI_WINDOWS)
#define FONT_PATH "C:\\Windows\\Fonts\\"
#else
#error "Unsupported platform"
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static uint64_t fontIds[2];
static TrackViewInfo s_trackViewInfo;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Editor_create()
{
	Emgui_create("foo");
	//fontIds[0] = Emgui_loadFont("/Users/daniel/Library/Fonts/MicroKnight_v1.0.ttf", 11.0f);
	fontIds[0] = Emgui_loadFont(FONT_PATH "Arial.ttf", 11.0f);

	memset(&s_trackViewInfo, 0, sizeof(s_trackViewInfo));

	RemoteConnection_createListner();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Editor_init()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Editor_update()
{
	Emgui_begin();

	TrackView_render(&s_trackViewInfo);

	Emgui_end();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Editor_keyDown(int key)
{
	bool handled_key = true;

	switch (key)
	{
		case EMGUI_ARROW_DOWN : break;
		case EMGUI_ARROW_UP : break;
		default : handled_key = false; break;
	}

	// do edit here

	if (key == '1')
	{
		return true;
	}

	return handled_key;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void processCommands()
{
	//SyncDocument *doc = trackView->getDocument();
	int strLen, newRow;
	//const struct sync_track* t;
	unsigned char cmd = 0;

	if (RemoteConnection_recv((char*)&cmd, 1, 0)) 
	{
		switch (cmd) 
		{
			case GET_TRACK:
			{
				char trackName[4096];

				memset(trackName, 0, sizeof(trackName));

				RemoteConnection_recv((char *)&strLen, sizeof(int), 0);
				strLen = ntohl(strLen);

				if (!RemoteConnection_connected())
					return;

				if (!RemoteConnection_recv(trackName, strLen, 0))
					return;

				rlog(R_INFO, "Got trackname %s (%d) from demo\n", trackName, strLen);

				// find track
				/*
				serverIndex = sync_find_track(doc, trackName.c_str());
				if (0 > serverIndex)
					serverIndex = int(doc->createTrack(trackName));

				// setup remap
				doc->clientSocket.clientTracks[trackName] = clientIndex++;

				// send key-frames
				t = doc->tracks[serverIndex];
				for (int i = 0; i < (int)t->num_keys; ++i)
					doc->clientSocket.sendSetKeyCommand(trackName, t->keys[i]);

				InvalidateRect(trackViewWin, NULL, FALSE);
				*/
				break;
			}


			case SET_ROW:
			{
				RemoteConnection_recv((char*)&newRow, sizeof(int), 0);
				//trackView->setEditRow(ntohl(newRow));
				break;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Editor_timedUpdate()
{
	RemoteConnection_updateListner();

	while (RemoteConnection_pollRead())
		processCommands();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void onOpen()
{
	LoadSave_loadRocketXMLDialog();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Editor_menuEvent(int menuItem)
{
	switch (menuItem)
	{
		//case EDITOR_MENU_NEW : onNew(); break;
		case EDITOR_MENU_OPEN : onOpen(); break;
		//case EDITOR_MENU_SAVE : onSave(); break;
		//case EDITOR_MENU_SAVE_AS : onSaveAs(); break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Editor_destroy()
{
}
