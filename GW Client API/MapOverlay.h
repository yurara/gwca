#ifndef _MAPOVERLAY_H
#define _MAPOVERLAY_H

struct MapOverlay {
	long index;
	float X;
	float Y;
	long unknown1; // = 0
	long unknown2; // = 0
	long option; // Affilitation/color. Enum { 0 = gray, blue, red, yellow, teal, purple, green, gray };
	long unknown3; // = 0
	long modelId; // Model of the displayed icon in the Minimap
	long unknown4; // = 0
	void* unknown5; // May concern the name
};

struct Quest {
	long Id; //Quest id
	long LogState;
	byte* unknown1;
	byte* unknown2;
	byte* unknown3;
	long MapFrom;
	float X; //Coordinates of the marker
	float Y;
	byte unknown4[8]; // = 0
	long MapTo;
	byte* unknown5;
	byte* unknown6;
};

#endif