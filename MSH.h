#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <string_view>
#include "Material.h"
#include "Model.h"
#include <bitset>
#include <regex>
#include <cstdint>

// Object that holds all data on the MSH as well as functions
// for all required operations
class MSH
{
public:
	
	// Sets the msh filename property
	void SetMSHFilename(std::string Fname);

	std::string GetMSHFilename();

	// Read the MSH into memory and populate MSH object
	bool ReadMSH();

	// Renames the selected material
	void RenameMaterial(unsigned short Selected, std::string name);

	// Toggles the selected flag from the selected material
	void SetFlag(unsigned short Selected, unsigned short Flag, unsigned short Value);

	// Sets the RenderType of the selected material
	void SetRT(unsigned short Selected, unsigned short RT);

	// Sets Data0 value of selected material
	void SetData0(unsigned short Selected, unsigned short V);

	// Sets Data1 value of selected material
	void SetData1(unsigned short Selected, unsigned short V);

	// Sets texture name of TX0D
	void SetTX0D(unsigned short Selected, std::string TexName);

	// Sets texture name of TX1D
	void SetTX1D(unsigned short Selected, std::string TexName);

	// Sets texture name of TX2D
	void SetTX2D(unsigned short Selected, std::string TexName);

	// Sets texture name of TX3D
	void SetTX3D(unsigned short Selected, std::string TexName);

	// Set BGRA value for diffuse 
	void SetDiffuseGBRA(unsigned short Selected, float RGBA[4]);

	// Set BGRA value for diffuse 
	void SetAmbientGBRA(unsigned short Selected, float RGBA[4]);

	// Set BGRA value for diffuse 
	void SetSpecularGBRA(unsigned short Selected, float RGBA[4]);

	// Set BGRA value for diffuse 
	void SetSpecularDecay(unsigned short Selected, unsigned int Value);

	// Renames the selected model
	void RenameModel(unsigned short Selected, std::string name);

	// Sets the parent of the modl
	void SetModelParent(unsigned short Selected, unsigned short ModelIndex);

	// Sets the visibility of the selected modl
	void SetModelVisibility(unsigned short Selected, unsigned short Visible);

	// Sets the MATI of the selected SEGM
	void SetClusterMaterial(unsigned short modl, unsigned short cluster, unsigned short material);

	// Sets the name of the CTEX
	void SetClothTex(unsigned short modl, std::string TexName);

	// Removes vertex colors from the modl
	void RemoveColors(unsigned short Selected);

	// Sets the vertex color CLRB chunk
	void SetCLRB(unsigned short Selected, unsigned short Cluster, unsigned short RGBA[4]);

	// Displays all models according to specifications
	void ListModels();

	// Displays all materials according to specifications
	void ListMaterials();

	// Make any needed adjustments/edits to Data unsigned char array before writing
	void PrepMSHForWrite();

	// Write the MSH object to file
	bool WriteMSH();

	// Returns whether the MSH has been altered and needs to be written
	bool MSHChanged();

	// Imports MODL chunk
	bool ImportMODL();

private:

	// String which holds the filename
	std::string FileName;

	// Size of the MSH file
	size_t Size = 0;

	// Char array that will hold data
	unsigned char* Data = nullptr;

	// String View used for all reading operations
	std::string_view sv;

	// Number of materials (MATD chunks attained from MATL chunk)
	uint32_t MaterialCount = 0;

	// Number of Models (MODL chunks)
	uint32_t ModelCount = 0;

	// Whether or not the model has been edited
	bool CHANGED = false;

	// Vector of material objects
	std::vector<Material> Materials;

	// Vector of model objects
	std::vector<Model> Models;

	// Vector that holds all MATD chunks
	std::vector<unsigned char> MATD_Chunks;

	// Vector that holds all MODL chunks
	std::vector<unsigned char> MODL_Chunks;

	// File positions for easy seeking
	size_t HEDR_Size = 0;
	size_t MATL_Count_Position = 0;
	size_t MATL_Position = 0;
	size_t MATL_Size = 0;
	size_t MSH2_Position = 0;
	size_t MSH2_Size = 0;
	size_t BLN2_Position = 0;
	size_t SKL2_Position = 0;
	size_t ANM2_Position = 0;

	// Read and save data concerning the material list
	void ReadMATL();

	// Populate the materials vector and save material info
	void ReadMATD();

	// Process DATA chunk for each material
	void ReadDATA();

	// Process ATRB chunk for each material
	void ReadATRB();

	// Process TX0D chunk for each material
	void ReadTX0D();

	// Process TX1D chunk for each material
	void ReadTX1D();

	// Process TX2D chunk for each material
	void ReadTX2D();

	// Process TX3D chunk for each material
	void ReadTX3D();

	// Populate the models vector and save modl info
	void ReadMODL();

	// Gets PRNT info on a parent MODL chunk
	void ReadPRNT();

	// Gets GEOM info on a parent MODL chunk
	void ReadGEOM();

	// Gets FLGS info from parent MODL chunk
	void ReadFLGS();

	// Gets CLTH info from parent MODL chunk
	void ReadCLTH();

	// Populates segment vector of MODL chunk
	void ReadSEGM();

	// Read MATI of each segment
	void ReadMATI();

	// Read CLRL of each segment if present
	void ReadCLRL();

	// Read CLRB of each segment if present
	void ReadCLRB();

	// Creates a new MATL chunk 
	std::vector<unsigned char> Create_MATL_Chunk();

	// Creates a new MATD chunk from a material object
	std::vector<unsigned char> Create_MATD_Chunk(Material Mat);

	// Creates a new MODL chunk from Model object
	std::vector<unsigned char> Create_MODL_Chunk(Model MODL);

	// Make adjustments just for Materials
	void PrepMatForWrite();

	// Make adjustments just for models
	void PrepModelForWrite();

	// Returns position of specified chunk, or 0 if not found
    size_t GetChunk(std::string header, size_t position);

	// Pads a string by multiple of four with nulls
	void PadString(std::vector<unsigned char>& str);

	// Pads a string by multiple of four with nulls
	void NullTerminate(std::vector<unsigned char>& str);

	// So that View can interact with the MSH
	friend class View;
};

// Creates a new MATL chunk to be written to file
inline std::vector<unsigned char> MSH::Create_MATL_Chunk()
{
	// Get MATL size by size of MATD chunks
	MATL_Size = (MATD_Chunks.size() + 4);

	// Create a vector of bytes to be the new MATL chunk
	std::vector<unsigned char> MATL_STR = { 'M', 'A', 'T', 'L' };

	// Temp arrays to read in 4 byte sizes
	char MATLSize[4] = { '\x00', '\x00', '\x00', '\x00' };
	char MATCount[4] = { '\x00', '\x00', '\x00', '\x00' };

	// Write to a stringstream the MATL chunk size, then read it
	std::stringstream s;
	s.setf(std::ios::binary);
	s.write(reinterpret_cast<char*>(&MATL_Size), 4);
	s.read(MATLSize, 4);

	// Add the MATL size we just got to the vector
	for (unsigned short a = 0; a < 4; a++)
		MATL_STR.push_back((unsigned char)MATLSize[a]);

	// Write to a stringstream the material count then read it
	std::stringstream t;
	t.setf(std::ios::binary);
	t.write(reinterpret_cast<char*>(&MaterialCount), 4);
	t.read(MATCount, 4);

	// Add the material count we just got
	for (unsigned short b = 0; b < 4; b++)
		MATL_STR.push_back(MATCount[b]);

	// Verbose output
	if (DEBUG)
		std::cout << "\n Create_MATL_Chunk: MATL chunk created! Size is "
		<< MATL_STR.size() << std::endl;

	return MATL_STR;
}

// Returns a MATD chunk as a unsigned char vector from a Material object
inline std::vector<unsigned char> MSH::Create_MATD_Chunk(Material Mat)
{
	// Only bother if it's changed. Otherwise just copy to vector and push it
	if (Mat.MATDChanged)
	{
		// Recalculate MATD_Size
		// Add together constants DATA_SIZE (52 + 4 + 4 = 60) and ATRB_SIZE (4 + 4 + 4 = 12) for sure
		// Add MatName_Size and Tex0, Tex1, Tex2, and Tex3 Sizes if applicable
		unsigned int TempSize = 72 + static_cast<size_t>(Mat.MatName_Size) + 8;

		if (Mat.TX0D_Size > 0 && !Mat.TX0D.empty())
			TempSize += static_cast<size_t>(Mat.TX0D_Size) + 8;

		if (Mat.TX1D_Size > 0 && !Mat.TX1D.empty())
			TempSize += static_cast<size_t>(Mat.TX1D_Size) + 8;

		if (Mat.TX2D_Size > 0 && !Mat.TX2D.empty())
			TempSize += static_cast<size_t>(Mat.TX2D_Size) + 8;

		if (Mat.TX3D_Size > 0 && !Mat.TX3D.empty())
			TempSize += static_cast<size_t>(Mat.TX3D_Size) + 8;

		Mat.MATD_Size = TempSize;

		// Start off MATD and NAME vectors
		std::vector<unsigned char> MATD_STR = { 'M', 'A', 'T', 'D' };
		std::vector<unsigned char> NAME_STR = { 'N', 'A', 'M', 'E' };

		// Temp arrays to read in sizes
		char MATDSize[4] = { '\x00', '\x00', '\x00', '\x00' };
		char NAMESize[4] = { '\x00', '\x00', '\x00', '\x00' };

		// Write to a stringstream the MATD chunk size, then read it
		std::stringstream s;
		s.setf(std::ios::binary);
		s.write(reinterpret_cast<char*>(&Mat.MATD_Size), 4);
		s.read(MATDSize, 4);

		// Write to a stringstream the NAME chunk size, then read it
		std::stringstream t;
		t.setf(std::ios::binary);
		t.write(reinterpret_cast<char*>(&Mat.MatName_Size), 4);
		t.read(NAMESize, 4);

		// Add the sizes we just got to our vectors
		for (unsigned short a = 0; a < 4; a++)
		{
			MATD_STR.push_back((unsigned char)MATDSize[a]);
			NAME_STR.push_back((unsigned char)NAMESize[a]);
		}

		// Add the name chunk to the MATD 
		for (unsigned short m = 0; m < 8; m++)
			MATD_STR.push_back(NAME_STR[m]);

		for (unsigned short b = 0; b < Mat.MatName_Size; b++)
			MATD_STR.push_back(Mat.MatName[b]);
		// Now we have a string MATD chunk up to DATA...

		// NOTE: Specular color must have a non-zero value for envmaps to appear!
		// Same deal for specular. So check if either and make default if at 0.0
		if (int(Mat.RenderType) == 6 || int(Mat.RenderType) == 4 || std::get<1>(Mat.MatFlags[0]))
			if (Mat.S_RGBA[0] == 0.0 || Mat.S_RGBA[1] == 0.0 || Mat.S_RGBA[2] == 0.0 || Mat.S_RGBA[3] == 0.0)
			{
				Mat.S_RGBA[0] = 0.7;
				Mat.S_RGBA[1] = 0.7;
				Mat.S_RGBA[2] = 0.7;
				Mat.S_RGBA[3] = 1.0;
			}

		std::vector<unsigned char> DATA_STR = { 'D', 'A', 'T', 'A', '\x34', '\x00', '\x00', '\x00' };

		// RGBA values as unsigned char arrays
		char Diff[4][4];
		char Spec[4][4];
		char Ambi[4][4];

		// v is RGBA value
		for (short v = 0; v < 4; v++)
		{
			// Ok now use stream trick again... 
			// For diffuse RGBA
			std::stringstream w;
			w.setf(std::ios::binary);
			w.write(reinterpret_cast<char*>(&Mat.D_RGBA[v]), 4);
			w.read(&Diff[v][0], 4);

			// For specular RGBA
			std::stringstream y;
			y.setf(std::ios::binary);
			y.write(reinterpret_cast<char*>(&Mat.S_RGBA[v]), 4);
			y.read(&Spec[v][0], 4);

			// For Ambient RGBA
			std::stringstream z;
			z.setf(std::ios::binary);
			z.write(reinterpret_cast<char*>(&Mat.A_RGBA[v]), 4);
			z.read(&Ambi[v][0], 4);
		}

		// Push back all our diffuse values
		for (short v = 0; v < 4; v++)
			for (short x = 0; x < 4; x++)
				DATA_STR.push_back(Diff[v][x]);

		// Push back all our specular values
		for (short v = 0; v < 4; v++)
			for (short x = 0; x < 4; x++)
				DATA_STR.push_back(Spec[v][x]);

		// Push back all our ambient values
		for (short v = 0; v < 4; v++)
			for (short x = 0; x < 4; x++)
				DATA_STR.push_back(Ambi[v][x]);

		// alright now finally push back the specular decay
		char SpecDecay[4] = { '\x00', '\x00', '\x00', '\x00' };

		std::stringstream z;
		z.setf(std::ios::binary);
		z.write(reinterpret_cast<char*>(&Mat.S_Decay), 4);
		z.read(SpecDecay, 4);

		for (unsigned short v = 0; v < 4; v++)
			DATA_STR.push_back((unsigned char)SpecDecay[v]);

		// Now make the ATRB chunk
		std::vector<unsigned char> ATRB_STR = { 'A', 'T', 'R', 'B','\x04','\x00','\x00','\x00' };
		ATRB_STR.push_back(Mat.CalculateATRB());
		ATRB_STR.push_back(Mat.RenderType);
		ATRB_STR.push_back(Mat.Data0);
		ATRB_STR.push_back(Mat.Data1);

		// Now add TX0D chunks if applicable 
		std::vector<unsigned char> TX0D_STR;
		if (Mat.TX0D.size() > 0)
		{
			TX0D_STR = { 'T', 'X', '0', 'D' };
			char TX0DSize[4] = { '\x00', '\x00', '\x00', '\x00' };

			// Push size
			std::stringstream u;
			u.setf(std::ios::binary);
			u.write(reinterpret_cast<char*>(&Mat.TX0D_Size), 4);
			u.read(TX0DSize, 4);

			for (unsigned short d = 0; d < 4; d++)
			{
				TX0D_STR.push_back((unsigned char)TX0DSize[d]);
			}

			// Push name to string
			for (unsigned char ch : Mat.TX0D)
				TX0D_STR.push_back(ch);
		}

		// Now add TX1D chunks if applicable 
		std::vector<unsigned char> TX1D_STR;
		if (Mat.TX1D.size() > 0)
		{
			TX1D_STR = { 'T', 'X', '1', 'D' };
			char TX1DSize[4] = { '\x00', '\x00', '\x00', '\x00' };

			// Push size
			std::stringstream u;
			u.setf(std::ios::binary);
			u.write(reinterpret_cast<char*>(&Mat.TX1D_Size), 4);
			u.read(TX1DSize, 4);

			for (unsigned short d = 0; d < 4; d++)
			{
				TX1D_STR.push_back((unsigned char)TX1DSize[d]);
			}

			// Push name to string
			for (unsigned char ch : Mat.TX1D)
				TX1D_STR.push_back(ch);
		}

		// Now add TX2D chunks if applicable 
		std::vector<unsigned char> TX2D_STR;
		if (Mat.TX2D.size() > 0)
		{
			TX2D_STR = { 'T', 'X', '2', 'D' };
			char TX2DSize[4] = { '\x00', '\x00', '\x00', '\x00' };

			// Push size
			std::stringstream u;
			u.setf(std::ios::binary);
			u.write(reinterpret_cast<char*>(&Mat.TX2D_Size), 4);
			u.read(TX2DSize, 4);

			for (unsigned short d = 0; d < 4; d++)
			{
				TX2D_STR.push_back((unsigned char)TX2DSize[d]);
			}

			// Push name to string
			for (unsigned char ch : Mat.TX2D)
				TX2D_STR.push_back(ch);
		}

		// Now add TX3D chunks if applicable 
		std::vector<unsigned char> TX3D_STR;
		if (Mat.TX3D.size() > 0)
		{
			TX3D_STR = { 'T', 'X', '3', 'D' };
			char TX3DSize[4] = { '\x00', '\x00', '\x00', '\x00' };

			// Push size
			std::stringstream u;
			u.setf(std::ios::binary);
			u.write(reinterpret_cast<char*>(&Mat.TX3D_Size), 4);
			u.read(TX3DSize, 4);

			for (unsigned short d = 0; d < 4; d++)
			{
				TX3D_STR.push_back((unsigned char)TX3DSize[d]);
			}

			// Push name to string
			for (unsigned char ch : Mat.TX3D)
				TX3D_STR.push_back(ch);
		}

		// All child chunks added! This is the complete MATD chunk in string
		std::vector<unsigned char> MATD;

		// Continue to push back all chunks into the MATD
		for (unsigned char l : MATD_STR)
			MATD.push_back(l);

		for (unsigned char f : DATA_STR)
			MATD.push_back(f);

		for (unsigned char g : ATRB_STR)
			MATD.push_back(g);

		if (!TX0D_STR.empty())
			for (unsigned char h : TX0D_STR)
				MATD.push_back(h);

		if (!TX1D_STR.empty())
			for (unsigned char i : TX1D_STR)
				MATD.push_back(i);

		if (!TX2D_STR.empty())
			for (unsigned char j : TX2D_STR)
				MATD.push_back(j);

		if (!TX3D_STR.empty())
			for (unsigned char k : TX3D_STR)
				MATD.push_back(k);

		// Verbose output
		if (DEBUG)
			std::cout << "\n Create_MATD_Chunk: MATD chunk created! Size is "
			<< MATD.size() << std::endl;

		return MATD;
	}
	else
	{
		uint32_t TempSize = Mat.MATD_Size + 8;
		std::vector<unsigned char> MATD;
		MATD.reserve(TempSize);
		std::string_view chunk = sv.substr((Mat.MATD_Position - 4), TempSize);
		for (size_t ch = 0; ch < TempSize; ch++)
			MATD.push_back((unsigned char)chunk[ch]);

		// Verbose output
		if (DEBUG)
			std::cout << "\n Create_MATD_Chunk: MATD chunk created! Size is "
			<< MATD.size() << std::endl;

		return MATD;
	}
}

// Returns a MODL chunk as a unsigned char vector from a Model object
// TODO: have to find way to remove CLRL and CLRB chunks!
inline std::vector<unsigned char> MSH::Create_MODL_Chunk(Model MODL)
{
	uint32_t TempSize = (uint32_t)MODL.MODL_Size + 8;

	// Our new MODL chunk that we'll operate on and return
	std::vector<unsigned char> MODEL;
	MODEL.reserve(TempSize);

	// Create substring of the MODL chunk 
	std::string_view chunk = sv.substr( (MODL.MODL_Position - 4), TempSize);

	// Now copy it over to the vector
	for (size_t ch = 0; ch < TempSize; ch++)
		MODEL.push_back((unsigned char)chunk[ch]);

	// So instead of getchunk use std::find in the sv chunk
	// Only bother if it's changed, otherwise just pack it and push it
	if (MODL.MODLChanged)
	{
		// If the NAME has changed...
		if (MODL.CHANGED[1])
		{
			size_t pos = chunk.find("NAME", 8) + 4;
			unsigned int OldNAMESize = MODL.OG_Value[1];
			signed long long Difference = OldNAMESize - MODL.Name_Size;

			char NAMESize[4] = { '\x00', '\x00', '\x00', '\x00' };

			// Write to a stringstream the NAME chunk size, then read it to NAMESize
			std::stringstream t;
			t.setf(std::ios::binary);
			t.write(reinterpret_cast<char*>(&MODL.Name_Size), 4);
			t.read(NAMESize, 4);

			// Simply overwrite our NAME size to the vector
			for (short C = 0; C < 4; C++)
				MODEL[pos + C] = (unsigned char)NAMESize[C];

			// Push up position to NAME
			pos += 4;

			// Erase old name from the vector
			MODEL.erase((MODEL.begin() + pos), (MODEL.begin() + pos + OldNAMESize));

			// Insert new name into the vector
			for (unsigned short C = 0; C < MODL.Name_Size; C++)
				MODEL.insert((MODEL.begin() + pos + C), MODL.Name[C]);

			// Increase positions of items after NAME chunk
			// If New name is smaller...
			if (Difference > 0)
			{
				MODL.PRNT_Position -= static_cast<size_t>(std::abs(Difference));
				MODL.FLGS_Position -= static_cast<size_t>(std::abs(Difference));
				MODL.CTEX_Position -= static_cast<size_t>(std::abs(Difference));
				for (unsigned short E = 0; E < MODL.Segments.size(); E++)
				{
					MODL.Segments.at(E).SEGM_Position -= static_cast<size_t>(std::abs(Difference));
					MODL.Segments.at(E).MATI_Position -= static_cast<size_t>(std::abs(Difference));
					MODL.Segments.at(E).CLRB_Position -= static_cast<size_t>(std::abs(Difference));
					MODL.Segments.at(E).CLRL_Position -= static_cast<size_t>(std::abs(Difference));
				}
			}
			// If New name is larger
			else if (Difference < 0)
			{
				MODL.PRNT_Position += static_cast<size_t>(std::abs(Difference));
				MODL.FLGS_Position += static_cast<size_t>(std::abs(Difference));
				MODL.CTEX_Position += static_cast<size_t>(std::abs(Difference));
				for (unsigned short E = 0; E < MODL.Segments.size(); E++)
				{
					MODL.Segments.at(E).SEGM_Position += static_cast<size_t>(std::abs(Difference));
					MODL.Segments.at(E).MATI_Position += static_cast<size_t>(std::abs(Difference));
					MODL.Segments.at(E).CLRB_Position += static_cast<size_t>(std::abs(Difference));
					MODL.Segments.at(E).CLRL_Position += static_cast<size_t>(std::abs(Difference));
				}
			}
		}

		// If the PRNT has changed...
		if (MODL.CHANGED[0])
		{
			// If there even was a PRNT
			if (MODL.OG_Value > 0)
			{
				size_t pos = chunk.find("PRNT", 8) + 4;
				unsigned int OldPRNTSize = MODL.OG_Value[0];
				signed long Difference = OldPRNTSize - MODL.PRNT_Size;
				char PRNTSize[4] = { '\x00', '\x00', '\x00', '\x00' };

				// Write to a stringstream the PRNT chunk size, then read it to PRNTSize
				std::stringstream t;
				t.setf(std::ios::binary);
				t.write(reinterpret_cast<char*>(&MODL.PRNT_Size), 4);
				t.read(PRNTSize, 4);

				// Simply overwrite our PRNT size to the vector
				for (short C = 0; C < 4; C++)
					MODEL[pos + C] = (unsigned char)PRNTSize[C];

				// Push up position to PRNT
				pos += 4;

				// Erase old name from the vector
				MODEL.erase((MODEL.begin() + pos), (MODEL.begin() + pos + OldPRNTSize));

				// Insert new name into the vector
				for (unsigned short C = 0; C < MODL.PRNT_Size; C++)
					MODEL.insert((MODEL.begin() + pos + C), MODL.PRNT[C]);

				// Increase positions of items after PRNT chunk
				// If New name is smaller...
				if (Difference > 0)
				{
					MODL.FLGS_Position -= std::abs(Difference);
					MODL.CTEX_Position -= std::abs(Difference);
					for (unsigned short E = 0; E < MODL.Segments.size(); E++)
					{
						MODL.Segments.at(E).SEGM_Position -= static_cast<size_t>(std::abs(Difference));
						MODL.Segments.at(E).MATI_Position -= static_cast<size_t>(std::abs(Difference));
						MODL.Segments.at(E).CLRB_Position -= static_cast<size_t>(std::abs(Difference));
						MODL.Segments.at(E).CLRL_Position -= static_cast<size_t>(std::abs(Difference));
					}
				}
				// If New name is larger
				else if (Difference < 0)
				{
					MODL.FLGS_Position += std::abs(Difference);
					MODL.CTEX_Position += std::abs(Difference);
					for (unsigned short E = 0; E < MODL.Segments.size(); E++)
					{
						MODL.Segments.at(E).SEGM_Position -= static_cast<size_t>(std::abs(Difference));
						MODL.Segments.at(E).MATI_Position -= static_cast<size_t>(std::abs(Difference));
						MODL.Segments.at(E).CLRB_Position -= static_cast<size_t>(std::abs(Difference));
						MODL.Segments.at(E).CLRL_Position -= static_cast<size_t>(std::abs(Difference));
					}
				}
			}
			else
			{
				// Newly made chunk will be inserted after NAME chunk
				size_t pos = chunk.find("NAME", 8) + 4 + 4 + MODL.OG_Value[1];
				signed short Difference = MODL.PRNT_Size;
				char PRNTSize[4] = { '\x00', '\x00', '\x00', '\x00' };
				std::vector<unsigned char> PRNT;
				PRNT.reserve(static_cast<size_t>(MODL.PRNT_Size) + 8);

				PRNT.push_back('P');
				PRNT.push_back('R');
				PRNT.push_back('N');
				PRNT.push_back('T');

				// Write to a stringstream the PRNT chunk size, then read it to PRNTSize
				std::stringstream t;
				t.setf(std::ios::binary);
				t.write(reinterpret_cast<char*>(&MODL.PRNT_Size), 4);
				t.read(PRNTSize, 4);

				// Push the PRNT size to PRNT chunk
				for (short C = 0; C < 4; C++)
					PRNT.push_back((unsigned char)PRNTSize[C]);

				// Push the PRNT name to the PRNT chunk
				for (unsigned short C = 0; C < MODL.PRNT_Size; C++)
					PRNT.push_back(MODL.PRNT[C]);

				unsigned short Size = MODL.PRNT_Size + 8;

				// Push the PRNT chunk to MODEL after NAME
				for (short C = 0; C < Size; C++)
					MODEL.insert((MODEL.begin() + pos + C), PRNT[C]);

				// Shift up the other positions
				MODL.FLGS_Position += Difference;
				MODL.CTEX_Position += Difference;
				for (unsigned short E = 0; E < MODL.Segments.size(); E++)
				{
					MODL.Segments.at(E).SEGM_Position -= static_cast<size_t>(std::abs(Difference));
					MODL.Segments.at(E).MATI_Position -= static_cast<size_t>(std::abs(Difference));
					MODL.Segments.at(E).CLRB_Position -= static_cast<size_t>(std::abs(Difference));
					MODL.Segments.at(E).CLRL_Position -= static_cast<size_t>(std::abs(Difference));
				}
			}
		}

		// If the FLGS has changed...
		if (MODL.CHANGED[2])
		{
			// This is unique in that we will rewrite it like MATD
			unsigned int OldFLGSValue = MODL.OG_Value[2];

			// If was there originally, minus 12 bytes from the position
			if (OldFLGSValue == 1 && !MODL.FLGS)
			{
				// Take us to header start
				size_t pos = chunk.find("FLGS", 8);
				MODEL.erase((MODEL.begin() + pos), (MODEL.begin() + pos + 12));

				// Shift back the positions of chunks after...
				MODL.CTEX_Position -= 12;
				for (unsigned short E = 0; E < MODL.Segments.size(); E++)
				{
					MODL.Segments.at(E).SEGM_Position -= static_cast<size_t>(std::abs(12));
					MODL.Segments.at(E).MATI_Position -= static_cast<size_t>(std::abs(12));
					MODL.Segments.at(E).CLRB_Position -= static_cast<size_t>(std::abs(12));
					MODL.Segments.at(E).CLRL_Position -= static_cast<size_t>(std::abs(12));
				}
			}
			// If was not there originally, insert 12 bytes of FLGS data after PRNT or NAME if no PRNT
			else if (OldFLGSValue == 0 && MODL.FLGS)
			{
				size_t pos;
				if (MODL.PRNT_Size > 0)
					pos = chunk.find("PRNT", 8) + 8 + MODL.PRNT_Size;
				else
					pos = chunk.find("NAME", 8) + 8 + MODL.Name_Size;


				unsigned char FLGS[12] = { 'F', 'L', 'G', 'S', '\x04', '\x00', '\x00', '\x00', '\x01', '\x00', '\x00', '\x00' };
				// Insert new FLGS chunk into the vector
				for (short C = 0; C < 12; C++)
					MODEL.insert((MODEL.begin() + pos + C), FLGS[C]);

				// Shift up positions of chunks after... CTEX that's you...
				MODL.CTEX_Position += 12;
				for (unsigned short E = 0; E < MODL.Segments.size(); E++)
				{
					MODL.Segments.at(E).SEGM_Position += static_cast<size_t>(std::abs(12));
					MODL.Segments.at(E).MATI_Position += static_cast<size_t>(std::abs(12));
					MODL.Segments.at(E).CLRB_Position += static_cast<size_t>(std::abs(12));
					MODL.Segments.at(E).CLRL_Position += static_cast<size_t>(std::abs(12));
				}
			}
		}

		// If the CTEX has changed...
		if (MODL.CHANGED[3])
		{
			size_t pos = chunk.find("CLTH", 8) + 12; // CTEX pos
			size_t pos2 = pos - 8; // CLTH pos
			size_t pos3 = chunk.find("GEOM", 4) + 12; // GEOM pos

			unsigned int OldNAMESize = MODL.OG_Value[3];
			signed long Difference = OldNAMESize - MODL.Name_Size;

			char NAMESize[4] = { '\x00', '\x00', '\x00', '\x00' };

			// Write to a stringstream the CTEX chunk size, then read it to NAMESize
			std::stringstream t;
			t.setf(std::ios::binary);
			t.write(reinterpret_cast<char*>(&MODL.CTEX_Size), 4);
			t.read(NAMESize, 4);

			// Simply overwrite our CTEX size to the vector
			for (short C = 0; C < 4; C++)
				MODEL[pos + C] = (unsigned char)NAMESize[C];

			// Push up position to CTEX
			pos += 4;

			// Erase old name from the vector
			MODEL.erase((MODEL.begin() + pos), (MODEL.begin() + pos + OldNAMESize));

			// Insert new name into the vector
			for (unsigned short C = 0; C < MODL.CTEX_Size; C++)
				MODEL.insert((MODEL.begin() + pos + C), MODL.CTEX[C]);

			// If New name is smaller...
			if (Difference > 0)
			{
				// Resize CLTH and GEOM headers
				MODL.CLTH_Size -= static_cast<size_t>(Difference);
				MODL.GEOM_Size -= static_cast<size_t>(Difference);

				// Offset future positions
				for (unsigned short E = 0; 0 < MODL.Segments.size(); E++)
				{
					MODL.Segments.at(E).CLRB_Position -= static_cast<size_t>(Difference);
					MODL.Segments.at(E).CLRL_Position -= static_cast<size_t>(Difference);
					MODL.Segments.at(E).MATI_Position -= static_cast<size_t>(Difference);
					MODL.Segments.at(E).SEGM_Position -= static_cast<size_t>(Difference);
				}
			}
			else if (Difference < 0)
			{
				// Resize CLTH and GEOM headers
				MODL.CLTH_Size += static_cast<size_t>(Difference);
				MODL.GEOM_Size += static_cast<size_t>(Difference);

				// Offset future positions
				for (unsigned short E = 0; 0 < MODL.Segments.size(); E++)
				{
					MODL.Segments.at(E).CLRB_Position += static_cast<size_t>(Difference);
					MODL.Segments.at(E).CLRL_Position += static_cast<size_t>(Difference);
					MODL.Segments.at(E).MATI_Position += static_cast<size_t>(Difference);
					MODL.Segments.at(E).SEGM_Position += static_cast<size_t>(Difference);
				}
			}

			// Now overwrite CLTH and GEOM sizes
			char CLTHSize[4] = { '\x00', '\x00', '\x00', '\x00' };

			// Write to a stringstream the CLTH chunk size, then read it
			std::stringstream a;
			a.setf(std::ios::binary);
			a.write(reinterpret_cast<char*>(&MODL.CLTH_Size), 4);
			a.read(CLTHSize, 4);

			// Simply overwrite our CLTH size to the vector
			for (short C = 0; C < 4; C++)
				MODEL[pos2 + C] = (unsigned char)CLTHSize[C];

			char GEOMSize[4] = { '\x00', '\x00', '\x00', '\x00' };

			// Write to a stringstream the GEOM chunk size, then read it
			std::stringstream b;
			b.setf(std::ios::binary);
			b.write(reinterpret_cast<char*>(&MODL.GEOM_Size), 4);
			b.read(GEOMSize, 4);

			// Simply overwrite our GEOM size to the vector
			for (short C = 0; C < 4; C++)
				MODEL[pos3 + C] = (unsigned char)GEOMSize[C];

		}

		// If the MATI has changed...
		if (MODL.CHANGED[4])
		{
			size_t AlreadyRead = 0;
			for (unsigned short C = 0; C < MODL.Segments.size(); C++)
			{
				size_t pos2 = chunk.find("MATI", AlreadyRead) + 8;
				char mati[4] = { '\x00', '\x00', '\x00', '\x00' };

				//Write to a std::string stream the MATI, then record it
				std::stringstream q;
				q.setf(std::ios::binary);
				q.write(reinterpret_cast<char*>(&MODL.Segments.at(C).MATI), 4);
				q.read(mati, 4);

				//Erase old MATI from the vector
				MODEL.erase((MODEL.begin() + pos2), (MODEL.begin() + pos2 + 4));

				//Insert new mati into the vector
				for (unsigned short d = 0; d < 4; d++)
					MODEL.insert((MODEL.begin() + pos2 + d), (unsigned char)mati[d]);

				AlreadyRead = pos2;
			}
		}

		// If the CLRB has changed...
		// NOTE: May need to resize GEOM and SEGM headers
		//TODO: MUCH
		if (MODL.CHANGED[5])
		{
			//Now insert/edit CLRB chunks
			for (unsigned short C = 0; C < MODL.Segments.size(); C++)
			{
				if (MODL.Segments.at(C).CLRB_Present)
				{
					// There's currently a CLRB to write!
					if (MODL.Segments.at(C).CLRB_OG)
					{
						// Find CLRB chunk position
						size_t pos2 = chunk.find("CLRB", std::abs((signed long long)MODL.MODL_Position - MODL.Segments.at(C).SEGM_Position));
	
						if (C != MODL.Segments.size() - 1)
						{
							// If not last segment
							if (pos2 < MODL.Segments.at(C + 1).SEGM_Position)
							{
								if (pos2 < Size)
								{
									// Verify in same SEGM chunk
									pos2 += 8; // Skip to CLRB value
	
									// Overwrite
									for (unsigned short D = 0; D < 4; D++)
										MODEL.at(pos2 + D) = MODL.Segments.at(C).CLRB[D];
								}
							}
						}
						else
						{
							if (pos2 < Size)
							{
								pos2 += 8; // Skip to CLRB value
								// Overwrite
								for (unsigned short D = 0; D < 4; D++)
									MODEL.at(pos2 + D) = MODL.Segments.at(C).CLRB[D];
							}
						}
					}
					else
					{
						// If it was not originally there, create it!
						unsigned char CLRB[12] = { 'C', 'L', 'R', 'B', '\x04', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00', '\x00' };
	
						// Push the CLRB in
						for (unsigned short D = 0; D < 4; D++)
							CLRB[8 + D] = MODL.Segments.at(C).CLRB[D];
	
						// Find insertion point
						size_t pos2 = chunk.find("UV0L", std::abs((signed long long)MODL.MODL_Position - MODL.Segments.at(C).SEGM_Position));
	
						if (C != MODL.Segments.size() - 1)
						{
							// If not last segment
							if (pos2 < (std::abs((signed long long)MODL.MODL_Position - MODL.Segments.at(C + 1).SEGM_Position)))
							{
								if (pos2 > Size) // Only if model has UVs should it have vertex colors
								{
									for (unsigned short E = 0; E < 12; E++)	// Insert CLRB chunk
										MODEL.insert(MODEL.begin() + pos2 + E, CLRB[E]);
	
	
									//Offset future positions by +12 bytes and resize heads SEGM and GEOM
									MODL.CTEX_Position += 12;
									MODL.Segments.at(C).SEGM_Size += 12;
									MODL.GEOM_Size += 12;

									size_t pos3 = chunk.find("GEOM", 4);
									// Resize the GEOM header
									char GEOMSize[4] = { '\x00', '\x00', '\x00', '\x00' };

									// Write to a stringstream the GEOM chunk size, then read it
									std::stringstream a;
									a.setf(std::ios::binary);
									a.write(reinterpret_cast<char*>(&MODL.GEOM_Size), 4);
									a.read(GEOMSize, 4);

									// Simply overwrite our GEOM size to the vector
									for (short C = 0; C < 4; C++)
										MODEL[pos3 + C] = (unsigned char)GEOMSize[C];

									// This should be the SEGM position
									size_t pos4 = std::abs((signed long long)MODL.MODL_Position - MODL.Segments.at(C).SEGM_Position);

									// Resize the SEGM header
									char SEGMSize[4] = { '\x00', '\x00', '\x00', '\x00' };

									// Write to a stringstream the SEGM chunk size, then read it
									std::stringstream c;
									c.setf(std::ios::binary);
									c.write(reinterpret_cast<char*>(&MODL.Segments.at(C).SEGM_Size), 4);
									c.read(SEGMSize, 4);

									// Simply overwrite our SEGM size to the vector
									for (short C = 0; C < 4; C++)
										MODEL[pos4 + C] = (unsigned char)SEGMSize[C];

									if (C != MODL.Segments.size() - 1)
									{
										// If not last segment
										for (unsigned short seg = C + 1; seg < MODL.Segments.size(); seg++)
										{
											MODL.Segments.at(seg).CLRB_Position += 12;
											MODL.Segments.at(seg).CLRL_Position += 12;
											MODL.Segments.at(seg).MATI_Position += 12;
											MODL.Segments.at(seg).SEGM_Position += 12;
										}
									}
								}
							}
						}
						else
						{
							if (pos2 > Size) // Only if model has UVs should it have vertex colors
							{
								for (unsigned short E = 0; E < 12; E++)	// Insert CLRB chunk
									MODEL.insert(MODEL.begin() + pos2 + E, CLRB[E]);
	
								//Offset future positions by +12 bytes
								MODL.CTEX_Position += 12;
								MODL.Segments.at(C).SEGM_Size += 12;
								MODL.GEOM_Size += 12;

								size_t pos3 = chunk.find("GEOM", 4);
								// Resize the GEOM header
								char GEOMSize[4] = { '\x00', '\x00', '\x00', '\x00' };

								// Write to a stringstream the GEOM chunk size, then read it
								std::stringstream a;
								a.setf(std::ios::binary);
								a.write(reinterpret_cast<char*>(&MODL.GEOM_Size), 4);
								a.read(GEOMSize, 4);

								// Simply overwrite our GEOM size to the vector
								for (short C = 0; C < 4; C++)
									MODEL[pos3 + C] = (unsigned char)GEOMSize[C];

								// This should be the SEGM position
								size_t pos4 = std::abs((signed long long)MODL.MODL_Position - MODL.Segments.at(C).SEGM_Position);

								// Resize the SEGM header
								char SEGMSize[4] = { '\x00', '\x00', '\x00', '\x00' };

								// Write to a stringstream the SEGM chunk size, then read it
								std::stringstream c;
								c.setf(std::ios::binary);
								c.write(reinterpret_cast<char*>(&MODL.Segments.at(C).SEGM_Size), 4);
								c.read(SEGMSize, 4);

								// Simply overwrite our SEGM size to the vector
								for (short C = 0; C < 4; C++)
									MODEL[pos4 + C] = (unsigned char)SEGMSize[C];
							}
						}
					}
				}
				else
				{
					// There shouldn't be a CLRB now
					if (MODL.Segments.at(C).CLRB_OG)
					{
						// But if there was one initially then we have to delete it!
						// Find CLRB chunk position
						size_t pos2 = chunk.find("CLRB", std::abs((signed long long)MODL.MODL_Position - MODL.Segments.at(C).SEGM_Position));

						// Erase 12 bytes
						MODEL.erase(MODEL.begin() + pos2, MODEL.begin() + pos2 + 12);

						// Resize GEOM and SEGM headers
						//Offset future positions by -12 bytes and resize heads SEGM and GEOM
						MODL.CTEX_Position -= 12;
						MODL.Segments.at(C).SEGM_Size -= 12;
						MODL.GEOM_Size -= 12;

						size_t pos3 = chunk.find("GEOM", 4);
						// Resize the GEOM header
						char GEOMSize[4] = { '\x00', '\x00', '\x00', '\x00' };

						// Write to a stringstream the GEOM chunk size, then read it
						std::stringstream a;
						a.setf(std::ios::binary);
						a.write(reinterpret_cast<char*>(&MODL.GEOM_Size), 4);
						a.read(GEOMSize, 4);

						// Simply overwrite our GEOM size to the vector
						for (short C = 0; C < 4; C++)
							MODEL[pos3 + C] = (unsigned char)GEOMSize[C];

						// This should be the SEGM position
						size_t pos4 = std::abs((signed long long)MODL.MODL_Position - MODL.Segments.at(C).SEGM_Position);

						// Resize the SEGM header
						char SEGMSize[4] = { '\x00', '\x00', '\x00', '\x00' };

						// Write to a stringstream the SEGM chunk size, then read it
						std::stringstream c;
						c.setf(std::ios::binary);
						c.write(reinterpret_cast<char*>(&MODL.Segments.at(C).SEGM_Size), 4);
						c.read(SEGMSize, 4);

						// Simply overwrite our SEGM size to the vector
						for (short C = 0; C < 4; C++)
							MODEL[pos4 + C] = (unsigned char)SEGMSize[C];

						if (C != MODL.Segments.size() - 1)
						{
							// If not last segment
							for (unsigned short seg = C + 1; seg < MODL.Segments.size(); seg++)
							{
								MODL.Segments.at(seg).CLRB_Position -= 12;
								MODL.Segments.at(seg).CLRL_Position -= 12;
								MODL.Segments.at(seg).MATI_Position -= 12;
								MODL.Segments.at(seg).SEGM_Position -= 12;
							}
						}
					}
				}
			}
		}
	
		// If the CLRL has changed...
		if (MODL.CHANGED[6])
		{
			// Now delete CLRL chunks
			for (unsigned short C = 0; C < MODL.Segments.size(); C++)
			{
				if (MODL.Segments.at(C).CLRL_OG == true)
				{
					if (MODL.Segments.at(C).CLRL_Present == false)
					{
						// CLRL has been removed, so delete it from the vector!
						signed long long MP = MODL.MODL_Position;
						signed long long CP = MODL.Segments.at(C).CLRL_Position - 4;
	
						// Find starting position
						size_t delstart = std::abs(MP - CP);
						size_t delstop = delstart + MODL.Segments.at(C).CLRL_Size + 8;
	
						MODEL.erase((MODEL.begin() + delstart), (MODEL.begin() + delstop));
	
						signed long long DST = delstart;
						signed long long DSP = delstop;
	
						size_t DS_Diff = std::abs(DST - DSP);
						MODL.CTEX_Position -= DS_Diff;
						if (C != MODL.Segments.size() - 1)
						{
							// If not last segment
							for (unsigned short seg = C + 1; seg < MODL.Segments.size(); seg++)
							{
								MODL.Segments.at(seg).CLRB_Position -= DS_Diff;
								MODL.Segments.at(seg).CLRL_Position -= DS_Diff;
								MODL.Segments.at(seg).MATI_Position -= DS_Diff;
								MODL.Segments.at(seg).SEGM_Position -= DS_Diff;
							}
						}
					}
				}
			}
		}

		// Don't forget to assign the new size!
		MODL.MODL_Size = MODEL.size() - 8;

		// Now write the new MODL size
		size_t pos3 = 4;
		char MODLSize[4] = { '\x00', '\x00', '\x00', '\x00' };

		// Write to a stringstream the MODL chunk size, then read it to MODLSize
		std::stringstream r;
		r.setf(std::ios::binary);
		r.write(reinterpret_cast<char*>(&MODL.MODL_Size), 4);
		r.read(MODLSize, 4);

		// Erase old MODL size from the vector
		MODEL.erase((MODEL.begin() + pos3), (MODEL.begin() + pos3 + 4));

		// Insert new MODL size into the vector
		for (short e = 0; e < 4; e++)
			MODEL.insert((MODEL.begin() + pos3 + e), (unsigned char)MODLSize[e]);
	}

	// Alright! A new MODL chunk with our edits has been made!
	// Verbose output
	if (DEBUG)
		std::cout << "\n Create_MODL_Chunk: MODL chunk created! Size is "
		<< MODEL.size() << std::endl;

	return MODEL;
}

// Returns position of specified 4 character header, or 0 if not found
inline size_t MSH::GetChunk(std::string header = "HEDR", size_t position = 0) 
{
	if (!sv.empty())
	{
		// Search the header at the position specified in the file
		size_t ChunkPosition = 0;
		ChunkPosition = sv.find(header, position);
		if (ChunkPosition > Size)
			return 0;
		else
			return ChunkPosition;
	}
	else
	{
		// Alternative w/o string_view (much slower!)
		std::bitset<4> BITS;

		for (size_t a = position; a < Size - 4; a++)
		{
			for (size_t b = 0; b < 4; b++)
			{
				if (Data[a + b] == header.at(b))
					BITS.set(b);
				else
					BITS.reset();
			}

			if (BITS.all())
				return a;
		}

		return 0;
	}
}

// Read and save data concerning the material list
inline void MSH::ReadMATL()
{
	// Find the MATL chunk -There's only one
	size_t position = GetChunk("MATL");
	position += 4;
	
	MATL_Position = position;

	// Create substring of the MATL size 
	std::string_view tempsv3 = sv.substr(position, 4);

	// Write to a stringstream the MATL chunk size, then read it
	std::stringstream s;
	s.setf(std::ios::binary);
	s.write(tempsv3.data(), 4);
	s.read(reinterpret_cast<char*>(&MATL_Size), 4);

	position += 4;

	MATL_Count_Position = position;
	// Create substring of the MATL count 
	std::string_view tempsv = sv.substr(position, 4);

	// Write to a stringstream the MATL count, then read it to MaterialCount
	std::stringstream r;
	r.setf(std::ios::binary);
	r.write(tempsv.data(), 4);
	r.read(reinterpret_cast<char*>(&MaterialCount), 4);

	// Reserve space in vector for materials
	Materials.reserve(MaterialCount);

	// Const bool in Material.h
	if (DEBUG)
	{
		std::cout << " ReadMATL: Number of Materials from MATL: ";
		std::cout << MaterialCount;
	}
}

// Populate the materials vector and save material info
inline void MSH::ReadMATD()
{
	// Counter
	size_t Place = 0;

	// Now process the MATD chunks for each material
	for (unsigned short C = 0; C < MaterialCount; C++)
	{
		// Create a material obj 
		Material Mat;

		// Find the MATD chunk
		size_t pos = GetChunk("MATD", Place) + 4;

		Mat.MATD_Position = pos;
		
		// Create substring of the MATD chunk size 
		std::string_view temp = sv.substr(pos, 4);

		// Write to a std::string stream the MATD chunk size, then read it
		std::stringstream s;
		s.setf(std::ios::binary);
		s.write(temp.data(), 4);
		s.read(reinterpret_cast<char*>(&Mat.MATD_Size), 4);

		pos += 8;
		// Save the Name length position for easy seeking
		Mat.MatName_Position = pos;

		// Create substring of the material name size 
		std::string_view tempsv = sv.substr(pos, 4);
		unsigned int MatNameLength = 0;

		// Write to a std::string stream the material name size, then read it to MatNameLength
		std::stringstream q;
		q.setf(std::ios::binary);
		q.write(tempsv.data(), 4);
		q.read(reinterpret_cast<char*>(&MatNameLength), 4);

		// Update pos to stay synced
		pos += 4;

		char* MatName = new char[MatNameLength];

		// Now read the name from that length we got
		std::string_view tempsv2 = sv.substr(pos, MatNameLength);
		std::stringstream t;
		t.setf(std::ios::binary);
		t.write(tempsv2.data(), MatNameLength);
		t.read(MatName, MatNameLength);

		// Update pos to stay synced
		pos += MatNameLength;

		std::string Name;

		for (unsigned short S = 0; S < MatNameLength; S++)
			Name.push_back(MatName[S]);

		delete[] MatName;

		Mat.MatName = Name;
		Mat.MatName_Size = MatNameLength;
		Mat.MATI = C;

		// Push back the material object to the vector of materials
		Materials.push_back(Mat);

		// Repeat loop from after we've already searched + some housekeeping
		Place = pos;
	}

	// Const bool in Material.h
	if (DEBUG)
	{
		for (auto m : Materials)
			std::cout << "\n ReadMATD: Material " << m.MatName << " Found!";
	}
}

// Process DATA chunk for each material
inline void MSH::ReadDATA()
{
	size_t Place = 0;

	// Now process the DATA chunks for each material
	for (unsigned short C = 0; C < MaterialCount; C++)
	{
		Place = Materials.at(C).MATD_Position;

		size_t position = GetChunk("DATA", Place);

		// Go to first RGBA section
		position += 8;
		Materials.at(C).DATA_Position = position;
		
		// Temp 2D array (Diffuse, then Specular, then Ambient) and the RGBA floats for each
		float RGBA[3][4];

		// OK wow this was hard to write...

		// Iterate for each RGBA series
		for (short a = 0; a < 3; a++)
		{
			// Create substring of the current RGBA (diff, spec, or amb)
			std::string_view temp = sv.substr(position, 16);
			short count = 0;

			// Iterate for each float color value
			for (short b = 0; b < 4; b++)
			{
				// Create substring of each color
				std::string_view temp2 = temp.substr(count, 4);

				// Write to a stream each color value then read it
				std::stringstream s;
				s.setf(std::ios::binary);
				s.write(temp2.data(), 4);
				s.read(reinterpret_cast<char*>(&RGBA[a][b]), 4);

				// Onto next color
				count += 4;
			}
			
			// Onto next RGBA series
			position += 16;
		}

		// RGBA should now be full, so populate diffuse RGBA
		for (short c = 0; c < 4; c++)
			Materials.at(C).D_RGBA[c] = RGBA[0][c];

		// RGBA should now be full, so populate specular RGBA
		for (short d = 0; d < 4; d++)
			Materials.at(C).S_RGBA[d] = RGBA[1][d];

		// RGBA should now be full, so populate ambient RGBA
		for (short e = 0; e < 4; e++)
			Materials.at(C).A_RGBA[e] = RGBA[2][e];

		// Alright! Now get that elusive specular decay value...
		// Create substring of spec decay value
		std::string_view temp3 = sv.substr(position, 4);

		// Write to a stream spec decay then read
		std::stringstream s;
		s.setf(std::ios::binary);
		s.write(temp3.data(), 4);
		s.read(reinterpret_cast<char*>(&Materials.at(C).S_Decay), 4);

		// FINISHED!

		// Const bool in Material.h
		if (DEBUG)
		{
			std::cout << "\n ReadDATA: Diffuse RGBA: ";
			for (short f = 0; f < 4; f++)
				std::cout << std::fixed << std::setprecision(2) << Materials.at(C).D_RGBA[f] << ' ';

			std::cout << "  Specular RGBA: ";
			for (short g = 0; g < 4; g++)
				std::cout << std::fixed << std::setprecision(2) << Materials.at(C).S_RGBA[g] << ' ';

			std::cout << "  Ambient RGBA: ";
			for (short h = 0; h < 4; h++)
				std::cout << std::fixed << std::setprecision(2) << Materials.at(C).A_RGBA[h] << ' ';

			std::cout << "\n ReadDATA: Specular Decay: " << std::fixed << std::setprecision(0) << (Materials.at(C).S_Decay);
		}
	}
}

// Process ATRB chunk for each material
inline void MSH::ReadATRB()
{
	// Counter
	size_t Place = 0;

	// Now process the ATRB chunks for each material
	for (unsigned short C = 0; C < MaterialCount; C++)
	{
		size_t position = GetChunk("ATRB", Place);

		Materials.at(C).ATRB_Position = position + 8;

		// Go to ATRB value position
		position += 8;

		// Read in ATRB as a single byte
		unsigned char ATRB = sv.at(position);
		position++;

		// Calculate the material flags
		Materials.at(C).CalculateFlags(ATRB);

		// Now read RenderType
		Materials.at(C).RenderType = sv.at(position);
		position++;

		// Now read Data0
		Materials.at(C).Data0 = sv.at(position);
		position++;

		// Now read Data1
		Materials.at(C).Data1 = sv.at(position);
		position++;

		Place = position;

		// Const bool in Material.h
		if (DEBUG)
		{
			std::cout << "\n ReadATRB: Rendertype: " << Material::RenderTypeName[Materials.at(C).RenderType];
			std::cout << "\t Data0: " << int(Materials.at(C).Data0);
			std::cout << "\t Data1: " << int(Materials.at(C).Data1);
		}
	}
}

// Process TX0D chunk for each material
inline void MSH::ReadTX0D()
{
	// Counter as we traverse the file
	size_t AlreadyRead = 0;

	// Iterate through all materials 
	for (unsigned short C = 0; C < MaterialCount; C++)
	{
		// Find the first TX0D chunk
		size_t pos = GetChunk("TX0D", AlreadyRead);
		if (pos != 0)
		{
			if (C == MaterialCount - 1)
			{
				// Move past header to name size
				pos += 4;

				// Record TexSize position
				Materials.at(C).TX0D_Position = pos;

				// Create substring of the texture name size 
				std::string_view tempsv = sv.substr(pos, 4);
				unsigned short NameLen = 0;

				// Write to a string stream the material name size, then read it to Tex NameLen
				std::stringstream s;
				s.setf(std::ios::binary);
				s.write(tempsv.data(), 4);
				s.read(reinterpret_cast<char*>(&NameLen), 4);

				// Move pos to name start
				pos += 4;

				char* TexName = new char[NameLen];

				// Now read from that length we got and record it
				std::string_view tempsv2 = sv.substr(pos, NameLen);
				std::stringstream t;
				t.setf(std::ios::binary);
				t.write(tempsv2.data(), NameLen);
				t.read(TexName, NameLen);

				// Set tex name property + some housekeeping
				std::string Name;

				for (unsigned short S = 0; S < NameLen; S++)
					Name.push_back(TexName[S]);

				delete[] TexName;

				Materials.at(C).TX0D = Name;
				Materials.at(C).TX0D_Size = NameLen;

				AlreadyRead = pos;
			}
			else
			{
				// If and only if this is in the same MATD chunk
				if (pos < Materials.at(C + 1).MatName_Position)
				{
					// Move past header to name size
					pos += 4;

					// Record TexSize position
					Materials.at(C).TX0D_Position = pos;

					// Create substring of the texture name size 
					std::string_view tempsv = sv.substr(pos, 4);
					unsigned short NameLen = 0;

					// Write to a std::string stream the material name size, then read it to Tex NameLength
					std::stringstream s;
					s.setf(std::ios::binary);
					s.write(tempsv.data(), 4);
					s.read(reinterpret_cast<char*>(&NameLen), 4);

					// Update pos to stay synced
					pos += 4;

					char* TexName = new char[NameLen];

					// Now read from that length we got and record it
					std::string_view tempsv2 = sv.substr(pos, NameLen);
					std::stringstream t;
					t.setf(std::ios::binary);
					t.write(tempsv2.data(), NameLen);
					t.read(TexName, NameLen);

					// Set tex name property + some housekeeping
					std::string Name;

					for (unsigned short S = 0; S < NameLen; S++)
						Name.push_back(TexName[S]);

					delete[] TexName;

					Materials.at(C).TX0D = Name;
					Materials.at(C).TX0D_Size = NameLen;

					AlreadyRead = pos;
				}
			}
		}
		else
			break;
	}
}

// Process TX1D chunk for each material
inline void MSH::ReadTX1D()
{
	// Counter as we traverse the file
	size_t AlreadyRead = 0;

	// Iterate through all materials 
	for (unsigned short C = 0; C < MaterialCount; C++)
	{
		// Find the first TX1D chunk
		size_t pos = GetChunk("TX1D", AlreadyRead);
		if (pos != 0)
		{
			if (C == MaterialCount - 1)
			{
				// Move past header to name size
				pos += 4;

				// Record TexSize position
				Materials.at(C).TX1D_Position = pos;

				// Create substring of the texture name size 
				std::string_view tempsv = sv.substr(pos, 4);
				unsigned short NameLen = 0;

				// Write to a string stream the material name size, then read it to Tex NameLen
				std::stringstream s;
				s.setf(std::ios::binary);
				s.write(tempsv.data(), 4);
				s.read(reinterpret_cast<char*>(&NameLen), 4);

				// Move pos to name start
				pos += 4;

				char* TexName = new char[NameLen];

				// Now read from that length we got and record it
				std::string_view tempsv2 = sv.substr(pos, NameLen);
				std::stringstream t;
				t.setf(std::ios::binary);
				t.write(tempsv2.data(), NameLen);
				t.read(TexName, NameLen);

				// Set tex name property + some housekeeping
				std::string Name;

				for (unsigned short S = 0; S < NameLen; S++)
					Name.push_back(TexName[S]);

				delete[] TexName;

				Materials.at(C).TX1D = Name;
				Materials.at(C).TX1D_Size = NameLen;

				AlreadyRead = pos;
			}
			else
			{
				// If and only if this is in the same MATD chunk
				if (pos < Materials.at(C + 1).MatName_Position)
				{
					// Move past header to name size
					pos += 4;

					// Record TexSize position
					Materials.at(C).TX1D_Position = pos;

					// Create substring of the texture name size 
					std::string_view tempsv = sv.substr(pos, 4);
					unsigned short NameLen = 0;

					// Write to a std::string stream the material name size, then read it to Tex NameLength
					std::stringstream s;
					s.setf(std::ios::binary);
					s.write(tempsv.data(), 4);
					s.read(reinterpret_cast<char*>(&NameLen), 4);

					// Update pos to stay synced
					pos += 4;

					char* TexName = new char[NameLen];

					// Now read from that length we got and record it
					std::string_view tempsv2 = sv.substr(pos, NameLen);
					std::stringstream t;
					t.setf(std::ios::binary);
					t.write(tempsv2.data(), NameLen);
					t.read(TexName, NameLen);

					// Set tex name property + some housekeeping
					std::string Name;

					for (unsigned short S = 0; S < NameLen; S++)
						Name.push_back(TexName[S]);

					delete[] TexName;

					Materials.at(C).TX1D = Name;
					Materials.at(C).TX1D_Size = NameLen;

					AlreadyRead = pos;
				}
			}
		}
		else
			break;
	}
}

// Process TX2D chunk for each material
inline void MSH::ReadTX2D()
{
	// Counter as we traverse the file
	size_t AlreadyRead = 0;

	// Iterate through all materials 
	for (unsigned short C = 0; C < MaterialCount; C++)
	{
		// Find the first TX2D chunk
		size_t pos = GetChunk("TX2D", AlreadyRead);
		if (pos != 0)
		{
			if (C == MaterialCount - 1)
			{
				// Move past header to name size
				pos += 4;

				// Record TexSize position
				Materials.at(C).TX2D_Position = pos;

				// Create substring of the texture name size 
				std::string_view tempsv = sv.substr(pos, 4);
				unsigned short NameLen = 0;

				// Write to a string stream the material name size, then read it to Tex NameLen
				std::stringstream s;
				s.setf(std::ios::binary);
				s.write(tempsv.data(), 4);
				s.read(reinterpret_cast<char*>(&NameLen), 4);

				// Move pos to name start
				pos += 4;

				char* TexName = new char[NameLen];

				// Now read from that length we got and record it
				std::string_view tempsv2 = sv.substr(pos, NameLen);
				std::stringstream t;
				t.setf(std::ios::binary);
				t.write(tempsv2.data(), NameLen);
				t.read(TexName, NameLen);

				// Set tex name property + some housekeeping
				std::string Name;

				for (unsigned short S = 0; S < NameLen; S++)
					Name.push_back(TexName[S]);

				delete[] TexName;

				Materials.at(C).TX2D = Name;
				Materials.at(C).TX2D_Size = NameLen;

				AlreadyRead = pos;
			}
			else
			{
				// If and only if this is in the same MATD chunk
				if (pos < Materials.at(C + 1).MatName_Position)
				{
					// Move past header to name size
					pos += 4;

					// Record TexSize position
					Materials.at(C).TX2D_Position = pos;

					// Create substring of the texture name size 
					std::string_view tempsv = sv.substr(pos, 4);
					unsigned short NameLen = 0;

					// Write to a std::string stream the material name size, then read it to Tex NameLength
					std::stringstream s;
					s.setf(std::ios::binary);
					s.write(tempsv.data(), 4);
					s.read(reinterpret_cast<char*>(&NameLen), 4);

					// Update pos to stay synced
					pos += 4;

					char* TexName = new char[NameLen];

					// Now read from that length we got and record it
					std::string_view tempsv2 = sv.substr(pos, NameLen);
					std::stringstream t;
					t.setf(std::ios::binary);
					t.write(tempsv2.data(), NameLen);
					t.read(TexName, NameLen);

					// Set tex name property + some housekeeping
					std::string Name;

					for (unsigned short S = 0; S < NameLen; S++)
						Name.push_back(TexName[S]);

					delete[] TexName;

					Materials.at(C).TX2D = Name;
					Materials.at(C).TX2D_Size = NameLen;

					AlreadyRead = pos;
				}
			}
		}
		else
			break;
	}
}

// Process TX3D chunk for each material
inline void MSH::ReadTX3D()
{
	// Counter as we traverse the file
	size_t AlreadyRead = 0;

	// Iterate through all materials 
	for (unsigned short C = 0; C < MaterialCount; C++)
	{
		// Find the first TX3D chunk
		size_t pos = GetChunk("TX3D", AlreadyRead);
		if (pos != 0)
		{
			if (C == MaterialCount - 1)
			{
				// Move past header to name size
				pos += 4;

				// Record TexSize position
				Materials.at(C).TX3D_Position = pos;

				// Create substring of the texture name size 
				std::string_view tempsv = sv.substr(pos, 4);
				unsigned short NameLen = 0;

				// Write to a string stream the material name size, then read it to Tex NameLen
				std::stringstream s;
				s.setf(std::ios::binary);
				s.write(tempsv.data(), 4);
				s.read(reinterpret_cast<char*>(&NameLen), 4);

				// Move pos to name start
				pos += 4;

				char* TexName = new char[NameLen];

				// Now read from that length we got and record it
				std::string_view tempsv2 = sv.substr(pos, NameLen);
				std::stringstream t;
				t.setf(std::ios::binary);
				t.write(tempsv2.data(), NameLen);
				t.read(TexName, NameLen);

				// Set tex name property + some housekeeping
				std::string Name;

				for (unsigned short S = 0; S < NameLen; S++)
					Name.push_back(TexName[S]);

				delete[] TexName;

				Materials.at(C).TX3D = Name;
				Materials.at(C).TX3D_Size = NameLen;

				AlreadyRead = pos;
			}
			else
			{
				// If and only if this is in the same MATD chunk
				if (pos < Materials.at(C + 1).MatName_Position)
				{
					// Move past header to name size
					pos += 4;

					// Record TexSize position
					Materials.at(C).TX3D_Position = pos;

					// Create substring of the texture name size 
					std::string_view tempsv = sv.substr(pos, 4);
					unsigned short NameLen = 0;

					// Write to a std::string stream the material name size, then read it to Tex NameLength
					std::stringstream s;
					s.setf(std::ios::binary);
					s.write(tempsv.data(), 4);
					s.read(reinterpret_cast<char*>(&NameLen), 4);

					// Update pos to stay synced
					pos += 4;

					char* TexName = new char[NameLen];

					// Now read from that length we got and record it
					std::string_view tempsv2 = sv.substr(pos, NameLen);
					std::stringstream t;
					t.setf(std::ios::binary);
					t.write(tempsv2.data(), NameLen);
					t.read(TexName, NameLen);

					// Set tex name property + some housekeeping
					std::string Name;

					for (unsigned short S = 0; S < NameLen; S++)
						Name.push_back(TexName[S]);

					delete[] TexName;

					Materials.at(C).TX3D = Name;
					Materials.at(C).TX3D_Size = NameLen;

					AlreadyRead = pos;
				}
			}
		}
		else
			break;
	}
}

// Populate the models vector and save modl info
inline void MSH::ReadMODL()
{
	// Set starting file position to last MATD chunk (MODLS are always after this)
	// File position tracker (so it continues search down the file)
	size_t Place = Materials.at(MaterialCount - 1).ATRB_Position;

	// cur file position
	size_t pos = 0;
	ModelCount = 0;
	// Find and count modl chunks
	do
	{
		// Create new model object
		Model MODL;
		pos = GetChunk("MODL", Place) + 4;

		if (pos > 4)
		{
			ModelCount++;
			MODL.MODL_Position = pos;

			// Create substring of the MODL chunk size 
			std::string_view temp = sv.substr(pos, 4);

			// Write to a std::string stream the MODL chunk size, then read it
			std::stringstream s;
			s.setf(std::ios::binary);
			s.write(temp.data(), 4);
			s.read(reinterpret_cast<char*>(&MODL.MODL_Size), 4);

			// Increment pos to skip MTYP header and size (always 4 bytes)
			pos += 12; // Now we're at MTYP
			MODL.MTYP_Position = pos;

			// Create substring of the MODL MTYP  
			std::string_view temp2 = sv.substr(pos, 4);

			// Write to a std::string stream the MODL chunk size, then read it
			std::stringstream t;
			t.setf(std::ios::binary);
			t.write(temp2.data(), 4);
			t.read(reinterpret_cast<char*>(&MODL.MTYP), 4);

			// Increment pos to skip MNDX header and size (also always 4 bytes)
			pos += 12; // Now we're at MNDX
			MODL.MNDX_Position = pos;

			// Create substring of the MODL MNDX  
			std::string_view temp3 = sv.substr(pos, 4);

			// Write to a std::string stream the MODL MNDX
			std::stringstream u;
			u.setf(std::ios::binary);
			u.write(temp3.data(), 4);
			u.read(reinterpret_cast<char*>(&MODL.MNDX), 4);

			// Increment pos to go to Name chunk size
			pos += 8; // Now at Name size
			MODL.Name_Position = pos;

			// Create substring of the MODL Name size  
			std::string_view temp4 = sv.substr(pos, 4);
			unsigned short NameLen = 0;

			// Write to a std::string stream the MODL MNDX
			std::stringstream v;
			v.setf(std::ios::binary);
			v.write(temp4.data(), 4);
			v.read(reinterpret_cast<char*>(&NameLen), 4);

			// Increment pos to start of name
			pos += 4;

			char* MODLName = new char[NameLen];

			// Now read the name from that length we got
			std::string_view temp5 = sv.substr(pos, NameLen);

			std::stringstream w;
			w.setf(std::ios::binary);
			w.write(temp5.data(), NameLen);
			w.read(MODLName, NameLen);

			// Since we treated the string as an array we have to push back to do it "officially"
			std::string Name;
			for (unsigned short S = 0; S < NameLen; S++)
				Name.push_back(MODLName[S]);

			delete[] MODLName;

			MODL.Name = Name;
			MODL.Name_Size = NameLen;

			// Increment pos the length of the name
			pos += NameLen;

			// So go ahead and push this MODL to our vector
			Models.push_back(MODL);

			Place = pos;
		}
		else
			break;

	} while (pos > 4);

	// Const bool in Material.h
	if (DEBUG)
	{
		for (auto m : Models)
			std::cout << "\n ReadMODL: Model " << m.Name << " Found!";
	}
}

// Gets PRNT info from MODL chunk if present
inline void MSH::ReadPRNT()
{

	size_t AlreadyRead = 0;
	// For each MODL chunk
	for (unsigned short C = 0; C < ModelCount; C++)
	{
		// Find the first prnt chunk
		size_t pos = GetChunk("PRNT", AlreadyRead);
		if (pos != 0)
		{
			if (C != ModelCount - 1)
			{
				if (pos < Models.at(C + 1).MODL_Position)
				{
					// Skip header and record position at PRNT size
					pos += 4;
					Models.at(C).PRNT_Position = pos;

					// Create substd::string of the model name size 
					std::string_view tempsv = sv.substr(pos, 4);
					unsigned short NameLen = 0;

					// Write to a std::string stream the model name size, then read it to ModelNameLength
					std::stringstream s;
					s.setf(std::ios::binary);
					s.write(tempsv.data(), 4);
					s.read(reinterpret_cast<char*>(&NameLen), 4);

					// Update pos to stay synced
					pos += 4;

					char* PRNT_Name = new char[NameLen];

					// Now read from that length we got and record it
					std::string_view tempsv2 = sv.substr(pos, NameLen);
					std::stringstream t;
					t.setf(std::ios::binary);
					t.write(tempsv2.data(), NameLen);
					t.read(PRNT_Name, NameLen);

					std::string Name;

					for (unsigned short S = 0; S < NameLen; S++)
						Name.push_back(PRNT_Name[S]);

					delete[] PRNT_Name;

					Models.at(C).PRNT = Name;
					Models.at(C).PRNT_Size = NameLen;
					AlreadyRead = pos;
				}
			}
			else
			{
				// Skip header and record position at PRNT size
				pos += 4;
				Models.at(C).PRNT_Position = pos;

				// Create substd::string of the model name size 
				std::string_view tempsv = sv.substr(pos, 4);
				unsigned short NameLen = 0;

				// Write to a std::string stream the model name size, then read it to ModelNameLength
				std::stringstream s;
				s.setf(std::ios::binary);
				s.write(tempsv.data(), 4);
				s.read(reinterpret_cast<char*>(&NameLen), 4);

				// Update pos to stay synced
				pos += 4;

				char* PRNT_Name = new char[NameLen];

				// Now read from that length we got and record it
				std::string_view tempsv2 = sv.substr(pos, NameLen);
				std::stringstream t;
				t.setf(std::ios::binary);
				t.write(tempsv2.data(), NameLen);
				t.read(PRNT_Name, NameLen);

				std::string Name;

				for (unsigned short S = 0; S < NameLen; S++)
					Name.push_back(PRNT_Name[S]);

				delete[] PRNT_Name;

				Models.at(C).PRNT = Name;
				Models.at(C).PRNT_Size = NameLen;
				AlreadyRead = pos;
			}
		}
	}
}

// Gets GEOM chunk info from MODL chunk if present
inline void MSH::ReadGEOM()
{
	size_t AlreadyRead = 0;
	// For each MODL chunk
	for (unsigned short C = 0; C < ModelCount; C++)
	{
		// Find the first prnt chunk
		size_t pos = GetChunk("GEOM", AlreadyRead);
		if (pos != 0)
		{
			if (C != ModelCount - 1)
			{
				if (pos < Models.at(C + 1).MODL_Position)
				{
					// Skip header and record position at GEOM size
					pos += 4;
					Models.at(C).GEOM_Position = pos;

					// Create substd::string of the GEOM chunk size 
					std::string_view tempsv = sv.substr(pos, 4);

					// Write to a string stream the GEOM size
					std::stringstream s;
					s.setf(std::ios::binary);
					s.write(tempsv.data(), 4);
					s.read(reinterpret_cast<char*>(&Models.at(C).GEOM_Size), 4);

					AlreadyRead = pos;
				}
			}
			else
			{
				// Skip header and record position at GEOM size
				pos += 4;
				Models.at(C).GEOM_Position = pos;

				// Create substd::string of the GEOM chunk size 
				std::string_view tempsv = sv.substr(pos, 4);

				// Write to a string stream the GEOM size
				std::stringstream s;
				s.setf(std::ios::binary);
				s.write(tempsv.data(), 4);
				s.read(reinterpret_cast<char*>(&Models.at(C).GEOM_Size), 4);

				AlreadyRead = pos;
			}
		}
	}
}

// Gets FLGS info from MODL chunk
inline void MSH::ReadFLGS()
{
	// Counter as we traverse the file
	size_t AlreadyRead = 0;

	// Iterate through all MODLs 
	for (unsigned short C = 0; C < ModelCount; C++)
	{
		size_t pos = GetChunk("FLGS", AlreadyRead);

		// Only continue if a FLGS is found in the file (from this point)
		if (pos != 0)
		{
			// Skip header and go to FLGS
			pos += 8;

			// Special case for last model
			if (C == (ModelCount - 1))
			{
				// Record FLGS 
				Models.at(C).FLGS_Position = pos;
				Models.at(C).FLGS = bool(sv.at(pos));
				AlreadyRead = pos;
			}
			else
			{
				if (pos < Models.at(C + 1).MODL_Position)
				{
					// Record PRNT size position
					Models.at(C).FLGS_Position = pos;
					Models.at(C).FLGS = bool(sv.at(pos));
					AlreadyRead = pos;
				}
			}
		}
		else
			break;
	}
}

// Gets CLTH info from MODL chunk
inline void MSH::ReadCLTH()
{
	// If there's only one model then let's make it easier
	if (ModelCount == 1)
	{
		size_t pos = GetChunk("CLTH");

		if (pos != 0)
		{
			// Skip header and record position at CLTH size
			pos += 4;
			Models.at(0).CLTH_Position = pos;
			Models.at(0).CLTH = true;

			// Create substring of the Cloth chunk size 
			std::string_view tempsv = sv.substr(pos, 4);

			// Write to a string stream CLTH chunk size
			std::stringstream s;
			s.setf(std::ios::binary);
			s.write(tempsv.data(), 4);
			s.read(reinterpret_cast<char*>(&Models.at(0).CLTH_Size), 4);

			pos += 8; // Skip past CLTH size and CTEX header to CTEX name size position

			Models.at(0).CTEX_Position = pos;
			
			// Create substd::string of the Cloth texture name size 
			std::string_view tempsv3 = sv.substr(pos, 4);
			unsigned short NameLen = 0;

			// Write to a std::string stream the model name size, then read it to NameLen
			std::stringstream u;
			u.setf(std::ios::binary);
			u.write(tempsv3.data(), 4);
			u.read(reinterpret_cast<char*>(&NameLen), 4);

			// Update pos to stay synced
			pos += 4;

			char* Tex_Name = new char[NameLen];

			// Now read from that length we got and record it
			std::string_view tempsv2 = sv.substr(pos, NameLen);
			std::stringstream t;
			t.setf(std::ios::binary);
			t.write(tempsv2.data(), NameLen);
			t.read(Tex_Name, NameLen);

			std::string Name;

			for (unsigned short S = 0; S < NameLen; S++)
				Name.push_back(Tex_Name[S]);

			delete[] Tex_Name;

			Models.at(0).CTEX = Name;
			Models.at(0).CTEX_Size = NameLen;
		}
	}
	// Do it the hard way 
	else
	{
		// Counter as we traverse the file
		size_t AlreadyRead = 0;

		// Iterate through all MODLs 
		for (unsigned short C = 0; C < ModelCount; C++)
		{
			size_t pos = GetChunk("CTEX", AlreadyRead);

			// Only continue if a CTEX is found in the file (from this point)
			if (pos != 0)
			{
				// Skip header and go to CTEX size
				pos += 4;

				// Put it in a try/catch in case it's the last material
				try
				{
					// If and only if this is in the right chunk
					if (pos < Models.at(static_cast<size_t>(C) + 1).MODL_Position)
					{
						AlreadyRead = pos;
					}
				}
				catch (...)
				{
					;
				}

				// Special case for last material
				if (C == (ModelCount - 1))
				{
					// Record PRNT size position
					Models.at(C).CTEX_Position = pos;
					Models.at(C).CLTH = true;

					// Create substd::string of the texture name size 
					std::string_view tempsv = sv.substr(pos, 4);
					unsigned short NameLen = 0;

					// Write to a std::string stream the material name size, then read it to CTEX NameLength
					std::stringstream s;
					s.setf(std::ios::binary);
					s.write(tempsv.data(), 4);
					s.read(reinterpret_cast<char*>(&NameLen), 4);

					// Update pos to stay synced
					pos += 4;

					char* CTEXName = new char[NameLen];

					// Now read from that length we got and record it
					std::string_view tempsv2 = sv.substr(pos, NameLen);
					std::stringstream t;
					t.setf(std::ios::binary);
					t.write(tempsv2.data(), NameLen);
					t.read(CTEXName, NameLen);

					// Set PRNT name property + some housekeeping
					std::string Name;

					for (unsigned short S = 0; S < NameLen; S++)
						Name.push_back(CTEXName[S]);

					delete[] CTEXName;

					Models.at(C).CTEX = Name;
					Models.at(C).CTEX_Size = NameLen;
				}
				else
				{
					if (pos < Models.at(static_cast<size_t>(C) + 1).MODL_Position)
					{
						// Record CTEX size position
						Models.at(C).CTEX_Position = pos;
						Models.at(C).CLTH = true;

						// Create substd::string of the CTEX name size 
						std::string_view tempsv = sv.substr(pos, 4);
						unsigned short NameLen = 0;

						// Write to a std::string stream the material name size, then read it to CTEX NameLength
						std::stringstream s;
						s.setf(std::ios::binary);
						s.write(tempsv.data(), 4);
						s.read(reinterpret_cast<char*>(&NameLen), 4);

						// Update pos to stay synced
						pos += 4;

						char* CTEXName = new char[NameLen];

						// Now read from that length we got and record it
						std::string_view tempsv2 = sv.substr(pos, NameLen);
						std::stringstream t;
						t.setf(std::ios::binary);
						t.write(tempsv2.data(), NameLen);
						t.read(CTEXName, NameLen);

						// Set CTEX name property + some housekeeping
						std::string Name;

						for (unsigned short S = 0; S < NameLen; S++)
							Name.push_back(CTEXName[S]);

						delete[] CTEXName;
						Models.at(C).CTEX = Name;
						Models.at(C).CTEX_Size = NameLen;
					}
				}
			}
		}
	}
}

// Populate Segments vector for each model
inline void MSH::ReadSEGM()
{
	size_t AlreadyRead = 0;
	// For each MODL chunk
	for (unsigned short C = 0; C < ModelCount; C++)
	{
		size_t pos = GetChunk("SEGM", AlreadyRead);
		if (pos != 0)
		{
			if (C != ModelCount - 1)
			{
				while (pos < Models.at(C + 1).MODL_Position)
				{
					// Keep doing this while still before next MODL chunk
					Segment NewSEGM;

					// Skip to size and record position to Segment
					pos += 4;
					NewSEGM.SEGM_Position = pos;

					// Record size
					std::string_view temp = sv.substr(pos, 4);

					// Write to a std::string stream the SEGM size, then record it
					std::stringstream s;
					s.setf(std::ios::binary);
					s.write(temp.data(), 4);
					s.read(reinterpret_cast<char*>(&NewSEGM.SEGM_Size), 4);


					// Push the segment to this MODL
					Models.at(C).Segments.push_back(NewSEGM);
					AlreadyRead = pos;

					// Now that we got everything for this segm, go to next one!
					pos = GetChunk("SEGM", AlreadyRead);
				}
			}
			// Special case for last one
			else
			{
				while (pos != 0)
				{
					// Keep doing this while there are still segments
					Segment NewSEGM;

					// Skip to size and record position to Segment
					pos += 4;
					NewSEGM.SEGM_Position = pos;

					// Record size
					std::string_view temp = sv.substr(pos, 4);

					// Write to a std::string stream the SEGM size, then record it
					std::stringstream s;
					s.setf(std::ios::binary);
					s.write(temp.data(), 4);
					s.read(reinterpret_cast<char*>(&NewSEGM.SEGM_Size), 4);

					// Push the segment to the MODL
					Models.at(C).Segments.push_back(NewSEGM);

					// Now that we got everything for this segm, go to next one!
					pos = GetChunk("SEGM", pos);
				}
			}
		}
		else
			break;
	}
}

// Read MATI of each segment
inline void MSH::ReadMATI()
{
	// First for is MODL, second is for SEGM
	for (unsigned short C = 0; C < ModelCount; C++)
	{
		for (unsigned short D = 0; D < Models.at(C).Segments.size(); D++)
		{
			size_t matipos = GetChunk("MATI", Models.at(C).Segments.at(D).SEGM_Position);
			matipos += 8; // Skip to MATI value
			Models.at(C).Segments.at(D).MATI_Position = matipos;

			// Create substring of the MATI
			std::string_view temp = sv.substr(matipos, 4);

			// Write to a std::string stream the MATI, then record it
			std::stringstream s;
			s.setf(std::ios::binary);
			s.write(temp.data(), 4);
			s.read(reinterpret_cast<char*>(&Models.at(C).Segments.at(D).MATI), 4);
		}
	}
}

// Read CLRL of each segment if present
inline void MSH::ReadCLRL()
{
	// First for is MODL, second is for SEGM
	for (unsigned short C = 0; C < ModelCount; C++)
	{
		for (unsigned short D = 0; D < Models.at(C).Segments.size(); D++)
		{
			size_t clrlpos = GetChunk("CLRL", Models.at(C).Segments.at(D).SEGM_Position);
			if (clrlpos != 0)
			{
				if (D != Models.at(C).Segments.size() - 1)
				{
					// If and only if this is in the right SEGM
					if (clrlpos < Models.at(C).Segments.at(D + 1).SEGM_Position)
					{
						clrlpos += 4; // Skip to CLRL chunk size
						Models.at(C).Segments.at(D).CLRL_Position = clrlpos;

						// Create substring of the CLRL chunk size
						std::string_view temp = sv.substr(clrlpos, 4);

						// Write to a std::string stream the size, then record it
						std::stringstream s;
						s.setf(std::ios::binary);
						s.write(temp.data(), 4);
						s.read(reinterpret_cast<char*>(&Models.at(C).Segments.at(D).CLRL_Size), 4);

						clrlpos += 4; // Move to CLRL count

						// Create substring of CLRL list size
						std::string_view temp2 = sv.substr(clrlpos, 4);

						// Write to a std::string stream the size, then record it
						std::stringstream t;
						t.setf(std::ios::binary);
						t.write(temp2.data(), 4);
						t.read(reinterpret_cast<char*>(&Models.at(C).Segments.at(D).CLRL_Count), 4);

						clrlpos += 4; // Move to CLRL list

						// For each CLRL save it away
						for (size_t E = 0; E < Models.at(C).Segments.at(D).CLRL_Count; E++)
						{
							// Create substring of 4 byte CLRL RGBA item
							std::string_view temp3 = sv.substr(clrlpos, 4);

							unsigned char RGBA[4];
							// Write to a string stream the size, then record it
							std::stringstream u;
							u.setf(std::ios::binary);
							u.write(temp3.data(), 4);
							u.read(reinterpret_cast<char*>(&RGBA), 4);

							std::vector<unsigned char> V;
							for (unsigned short F = 0; F < 4; F++)
								V.push_back(RGBA[F]);
							Models.at(C).Segments.at(D).CLRL.push_back(V);

							clrlpos += 4; // Move to next RGBA item
						}
						Models.at(C).Segments.at(D).CLRL_Present = true;
						Models.at(C).Segments.at(D).CLRL_OG = true;
					}
				}
				else
				{
					// If and only if this isn't the last model
					if (C != ModelCount - 1)
					{
						if (clrlpos < Models.at(C + 1).MODL_Position)
						{
							// Special case for last one
							clrlpos += 4; // Skip to CLRL chunk size
							Models.at(C).Segments.at(D).CLRL_Position = clrlpos;

							// Create substring of the CLRL chunk size
							std::string_view temp = sv.substr(clrlpos, 4);

							// Write to a std::string stream the size, then record it
							std::stringstream s;
							s.setf(std::ios::binary);
							s.write(temp.data(), 4);
							s.read(reinterpret_cast<char*>(&Models.at(C).Segments.at(D).CLRL_Size), 4);

							clrlpos += 4; // Move to CLRL count

							// Create substring of CLRL list size
							std::string_view temp2 = sv.substr(clrlpos, 4);

							// Write to a std::string stream the size, then record it
							std::stringstream t;
							t.setf(std::ios::binary);
							t.write(temp2.data(), 4);
							t.read(reinterpret_cast<char*>(&Models.at(C).Segments.at(D).CLRL_Count), 4);

							clrlpos += 4; // Move to CLRL list

							// For each CLRL save it away
							for (size_t E = 0; E < Models.at(C).Segments.at(D).CLRL_Count; E++)
							{
								// Create substring of 4 byte CLRL RGBA item
								std::string_view temp3 = sv.substr(clrlpos, 4);

								unsigned char RGBA[4];
								// Write to a string stream the size, then record it
								std::stringstream u;
								u.setf(std::ios::binary);
								u.write(temp3.data(), 4);
								u.read(reinterpret_cast<char*>(&RGBA), 4);
								std::vector<unsigned char> V;
								for (unsigned short F = 0; F < 4; F++)
									V.push_back(RGBA[F]);
								Models.at(C).Segments.at(D).CLRL.push_back(V);

								clrlpos += 4; // Move to next RGBA item
							}
							Models.at(C).Segments.at(D).CLRL_Present = true;
							Models.at(C).Segments.at(D).CLRL_OG = true;
						}
					}
					else
					{
						// If last model and last segment
						clrlpos += 4; // Skip to CLRL chunk size
						Models.at(C).Segments.at(D).CLRL_Position = clrlpos;

						// Create substring of the CLRL chunk size
						std::string_view temp = sv.substr(clrlpos, 4);

						// Write to a std::string stream the size, then record it
						std::stringstream s;
						s.setf(std::ios::binary);
						s.write(temp.data(), 4);
						s.read(reinterpret_cast<char*>(&Models.at(C).Segments.at(D).CLRL_Size), 4);

						clrlpos += 4; // Move to CLRL count

						// Create substring of CLRL list size
						std::string_view temp2 = sv.substr(clrlpos, 4);

						// Write to a std::string stream the size, then record it
						std::stringstream t;
						t.setf(std::ios::binary);
						t.write(temp2.data(), 4);
						t.read(reinterpret_cast<char*>(&Models.at(C).Segments.at(D).CLRL_Count), 4);

						clrlpos += 4; // Move to CLRL list

						// For each CLRL save it away
						for (size_t E = 0; E < Models.at(C).Segments.at(D).CLRL_Count; E++)
						{
							// Create substring of 4 byte CLRL RGBA item
							std::string_view temp3 = sv.substr(clrlpos, 4);

							unsigned char RGBA[4];
							// Write to a string stream the size, then record it
							std::stringstream u;
							u.setf(std::ios::binary);
							u.write(temp3.data(), 4);
							u.read(reinterpret_cast<char*>(&RGBA), 4);
							std::vector<unsigned char> V;
							for (unsigned short F = 0; F < 4; F++)
								V.push_back(RGBA[F]);
							Models.at(C).Segments.at(D).CLRL.push_back(V);

							clrlpos += 4; // Move to next RGBA item
						}
						Models.at(C).Segments.at(D).CLRL_Present = true;
						Models.at(C).Segments.at(D).CLRL_OG = true;
					}
				}
			}
			else
				break;
		}
	}
}

// Read CLRB of each segment if present
inline void MSH::ReadCLRB()
{
	// First for is MODL, second is for SEGM
	for (unsigned short C = 0; C < ModelCount; C++)
	{
		for (unsigned short D = 0; D < Models.at(C).Segments.size(); D++)
		{
			size_t clrbpos = GetChunk("CLRB", Models.at(C).Segments.at(D).SEGM_Position);
			if (clrbpos != 0)
			{
				if (D != Models.at(C).Segments.size() - 1)
				{
					// If and only if this is in the right SEGM
					if (clrbpos < Models.at(C).Segments.at(D + 1).SEGM_Position)
					{
						clrbpos += 8; // Skip to CLRB value
						Models.at(C).Segments.at(D).CLRB_Position = clrbpos;
						Models.at(C).Segments.at(D).CLRB_OG = true;
						Models.at(C).Segments.at(D).CLRB_Present = true;

						// Create substring of the RGBA value
						std::string_view temp = sv.substr(clrbpos, 4);

						// Write to a std::string stream the MATI, then record it
						std::stringstream s;
						s.setf(std::ios::binary);
						s.write(temp.data(), 4);
						s.read(reinterpret_cast<char*>(&Models.at(C).Segments.at(D).CLRB), 4);
						clrbpos += 4;
					}
				}
				else
				{
					// If and only if this isn't the last model
					if (C != ModelCount - 1)
					{
						if (clrbpos < Models.at(C + 1).MODL_Position)
						{
							clrbpos += 8; // Skip to CLRB value
							Models.at(C).Segments.at(D).CLRB_Position = clrbpos;
							Models.at(C).Segments.at(D).CLRB_OG = true;
							Models.at(C).Segments.at(D).CLRB_Present = true;

							// Create substring of the RGBA value
							std::string_view temp = sv.substr(clrbpos, 4);

							// Write to a std::string stream the MATI, then record it
							std::stringstream s;
							s.setf(std::ios::binary);
							s.write(temp.data(), 4);
							s.read(reinterpret_cast<char*>(&Models.at(C).Segments.at(D).CLRB), 4);
							clrbpos += 4;
						}
					}
					else
					{
						clrbpos += 8; // Skip to CLRB value
						Models.at(C).Segments.at(D).CLRB_Position = clrbpos;
						Models.at(C).Segments.at(D).CLRB_OG = true;
						Models.at(C).Segments.at(D).CLRB_Present = true;

						// Create substring of the RGBA value
						std::string_view temp = sv.substr(clrbpos, 4);

						// Write to a std::string stream the MATI, then record it
						std::stringstream s;
						s.setf(std::ios::binary);
						s.write(temp.data(), 4);
						s.read(reinterpret_cast<char*>(&Models.at(C).Segments.at(D).CLRB), 4);
						clrbpos += 4;
					}
				}
			}
			else
				break;
		}
	}
}

// Reads MSH to vector of chars and performs all reading operations
inline bool MSH::ReadMSH()
{
	// Open File
	std::ifstream InFile(FileName.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	if (!InFile.is_open())
		return false;

	// Get the size of the MSH file by recording stream position (at end by ios::ate)
	Size = static_cast<size_t>(InFile.tellg());

	// Allocate a new unsigned char array of msh filesize and point MSHFile->Data to it
	Data = new unsigned char[Size];

	// For convenience :)
	if (DEBUG)
	{
		std::cout << " ReadMSH: MSH file successfully read into memory!\n";
		std::cout << " ReadMSH: Size of the MSH is " << Size << " bytes\n";
	}

	// Set the stream position to the beginning of the file
	InFile.seekg(InFile.beg);

	// Read file into the unsigned char array: Data[]
	InFile.read((char*)Data, Size);

	// Close file and flush buffers
	InFile.close();

	// This is how we will read data
	sv = std::string_view((char*)Data, Size);

	// Read Material Info ---------------------------------------
	// Read and save data concerning the material list
	ReadMATL();

	// Populate the materials vector and save material info
	ReadMATD();

	// Process the DATA chunk for each material
	ReadDATA();

	// Process ATRB chunk for each material
	ReadATRB();

	// Process TX0D chunk for each material
	ReadTX0D();

	// Process TX1D chunk for each material
	ReadTX1D();

	// Process TX1D chunk for each material
	ReadTX2D();

	// Process TX3D chunk for each material
	ReadTX3D();

	// Read Model Info ---------------------------------------
	// Read and save data for each MODL chunk and populate models vector
	ReadMODL();

	// Reads in Parent info chunk for each MODL chunk
	ReadPRNT();

	// Reads in Geometry info chunk for each MODL chunk
	ReadGEOM();

	// Reads in Flag info chunk for each MODL chunk
	ReadFLGS();

	// Reads and populates segments vector for each MODL chunk
	ReadSEGM();

	// Reads in MATI for each SEGM
	ReadMATI();

	// Reads in vertex colors for each SEGM
	ReadCLRL();

	// Reads in single vertex color for each SEGM
	ReadCLRB();

	// Checks to see if a model is a cloth and records it
	ReadCLTH();

	return true;
}

// Sets the msh filename property and checks that it exists
inline void MSH::SetMSHFilename(std::string Fname = "")
{
	if (Fname.empty())
	{
		// Const bool found in Material.h
		if (!DEBUG)
		{
			// Loop to ensure valid and correct input for filename
			// Sentinel
			bool Valid_Name = false;
			do
			{
				// Prompt user for filename of msh
				std::cout << " Enter MSH filename: ";
				std::string MSHFileName;

				// If input can be read, continue
				if (std::cin >> MSHFileName)
				{
					// Check if filename has .msh extension, if not add it
					if (std::regex_match(MSHFileName, std::regex("(.*)(\\.msh)") ) )
						// Assign the MSH object's Filename member to input
						FileName = MSHFileName;
					else
						FileName = MSHFileName + ".msh";

					// Check if file exists
					std::ifstream TestFile(FileName.c_str());
					if (TestFile.is_open())
					{
						Valid_Name = true;
						TestFile.close();
					}
					else
						std::cout << " Failed to open file! Please try again.\n\n";
				}
				else
					std::cout << " Error with input, please try again!\n\n";
			} while (!Valid_Name);

		}
		else
		{
			// JUST FOR TESTING ---------------------------------------------------
			FileName = Fname;
			// --------------------------------------------------------------------
		}
	}
	else
	{
		if (DEBUG)
			std::cout << Fname << "\n";

		// Check if filename has .msh extension, if not add it
		if (std::regex_match(Fname, std::regex("(.*)(\\.msh)")))
			// Assign the MSH object's Filename member to input
			FileName = Fname;
		else
			FileName = Fname + ".msh";
	}
		
}

// Returns MSH FileName as string
inline std::string MSH::GetMSHFilename()
{
	return FileName;
}

// Pads a vector of chars by multiple of four with nulls
inline void MSH::PadString(std::vector<unsigned char>& str)
{
	if (str.size() == 0)
		return;

	unsigned short i = 0;
	while (str.size() >= i)
		i += 4;

	unsigned short NumPads = i - static_cast<unsigned short>(str.size());
	for (unsigned short c = 0; c < NumPads; c++)
	{
		str.push_back('\x00');
	}
}

// Null-terminates a given vector of chars
inline void MSH::NullTerminate(std::vector<unsigned char>& str)
{
	str.push_back('\x00');
}

// Adjusts and prepares materials for writing to file
inline void MSH::PrepMatForWrite()
{
	if (MATD_Chunks.size() > 0)
		MATD_Chunks.clear();

	// count up for reserve
	size_t ToReserve = 0;
	for (unsigned short C = 0; C < MaterialCount; C++)
		ToReserve += Materials.at(C).MATD_Size;

	MATD_Chunks.reserve(ToReserve);

	// Iterate through each material and create a new MATD chunk for it
	for (unsigned short C = 0; C < MaterialCount; C++)
	{
		std::vector<unsigned char> m = Create_MATD_Chunk(Materials.at(C));
		for (auto ma : m)
			MATD_Chunks.push_back(ma);
	}

	// Creates a new MATL chunk
	std::vector<unsigned char> MATL = Create_MATL_Chunk();

	// Offset any positions necessary 
	size_t start = MATL_Position - 4;
	size_t end = Models.at(0).MODL_Position - 4;

	signed long long MaterialSection = end - start;
	signed long long NewMatSection = (MATD_Chunks.size() + MATL.size());
	signed long long difference = MaterialSection - NewMatSection;

	// If NewMatSection is smaller
	if (difference > 0)
	{
		for (unsigned short mdl = 0; mdl < ModelCount; mdl++)
		{
			Models.at(mdl).MODL_Position -= std::abs(difference);
			Models.at(mdl).MTYP_Position -= std::abs(difference);
			Models.at(mdl).MNDX_Position -= std::abs(difference);
			Models.at(mdl).Name_Position -= std::abs(difference);
			Models.at(mdl).PRNT_Position -= std::abs(difference);
			Models.at(mdl).FLGS_Position -= std::abs(difference);
			Models.at(mdl).CTEX_Position -= std::abs(difference);

			for (unsigned short seg = 0; seg < Models.at(mdl).Segments.size(); seg++)
			{
				Models.at(mdl).Segments.at(seg).MATI_Position -= std::abs(difference);
				Models.at(mdl).Segments.at(seg).CLRB_Position -= std::abs(difference);
				Models.at(mdl).Segments.at(seg).CLRL_Position -= std::abs(difference);
				Models.at(mdl).Segments.at(seg).SEGM_Position -= std::abs(difference);
			}
		}
	}
	// If NewMatSection is larger
	else if (difference < 0)
	{
		for (unsigned short mdl = 0; mdl < ModelCount; mdl++)
		{
			Models.at(mdl).MODL_Position += std::abs(difference);
			Models.at(mdl).MTYP_Position += std::abs(difference);
			Models.at(mdl).MNDX_Position += std::abs(difference);
			Models.at(mdl).Name_Position += std::abs(difference);
			Models.at(mdl).PRNT_Position += std::abs(difference);
			Models.at(mdl).FLGS_Position += std::abs(difference);
			Models.at(mdl).CTEX_Position += std::abs(difference);

			for (unsigned short seg = 0; seg < Models.at(mdl).Segments.size(); seg++)
			{
				Models.at(mdl).Segments.at(seg).MATI_Position += std::abs(difference);
				Models.at(mdl).Segments.at(seg).CLRB_Position += std::abs(difference);
				Models.at(mdl).Segments.at(seg).CLRL_Position += std::abs(difference);
				Models.at(mdl).Segments.at(seg).SEGM_Position += std::abs(difference);
			}
		}
	}

	// MSH data from HEDR to before MATL
	std::string_view before = sv.substr(0, start);

	// MSH data from MODL to EOF
	std::string_view after = sv.substr(end, (Size - end));

	std::vector<unsigned char> NewDataV;
	NewDataV.reserve((before.size() + MATL.size() + MATD_Chunks.size() + after.size()));

	// Now populate NewData with all our new stuff!
	for (size_t b = 0; b < before.size(); b++)
		NewDataV.push_back(before.at(b));

	// Insert new MATL and MATD chunks
	for (size_t l = 0; l < MATL.size(); l++)
		NewDataV.push_back(MATL.at(l));

	for (size_t d = 0; d < MATD_Chunks.size(); d++)
		NewDataV.push_back(MATD_Chunks.at(d));

	for (size_t a = 0; a < after.size(); a++)
		NewDataV.push_back(after.at(a));

	Size = NewDataV.size();

	delete[] Data;
	Data = new unsigned char[Size];

	for (size_t E = 0; E < Size; E++)
		Data[E] = NewDataV.at(E);

	NewDataV.clear();

	// Update sv
	sv = std::string_view((char*)Data, Size);

	// Verbose output
	if (DEBUG)
		std::cout << "\n PrepMatForWrite: Mat info is ready!\n";
}

// Adjusts and prepares models fro writing to file
inline void MSH::PrepModelForWrite()
{
	if (MODL_Chunks.size() > 0)
		MODL_Chunks.clear();

	// count up for reserve
	size_t ToReserve = 0;
	for (unsigned short C = 0; C < ModelCount; C++)
		ToReserve += Models.at(C).MODL_Size;

	MODL_Chunks.reserve(ToReserve);

	// Iterate through each model and create a new MODL chunk for it
	for (unsigned short C = 0; C < ModelCount; C++)
	{
		std::vector<unsigned char> m = Create_MODL_Chunk(Models.at(C));
		for (auto ma : m)
			MODL_Chunks.push_back(ma);
	}

	// Start at first MODL section
	size_t start = Models.at(0).MODL_Position - 4;
	size_t end = 0;

	// End before first appearance of these
	BLN2_Position = GetChunk("BLN2");
	SKL2_Position = GetChunk("SKL2");
	ANM2_Position = GetChunk("ANM2");
	size_t CL1L_Pos = Size - 8;

	// End at any of these chunks or just CL1L
	if (BLN2_Position > 0)
		end = BLN2_Position;
	else if (SKL2_Position > 0)
		end = SKL2_Position;
	else if (ANM2_Position > 0)
		end = ANM2_Position;
	else
		end = CL1L_Pos;

	// MSH data from HEDR to before MATL
	std::string_view before = sv.substr(0, start);

	// MSH data from MODL to EOF
	std::string_view after = sv.substr(end, (Size - end));

	std::vector<unsigned char> NewDataV;
	NewDataV.reserve((before.size() + MODL_Chunks.size() + after.size()));

	size_t place = 0;
	size_t counter = 0;
	// Now populate NewData with all our new stuff!
	for (size_t b = 0; b < before.size(); b++)
		NewDataV.push_back(before.at(b));

	for (size_t d = 0; d < MODL_Chunks.size(); d++)
		NewDataV.push_back(MODL_Chunks.at(d));

	for (size_t a = 0; a < after.size(); a++)
		NewDataV.push_back(after.at(a));

	Size = NewDataV.size();

	delete[] Data;
	Data = new unsigned char[Size];

	for (size_t E = 0; E < Size; E++)
		Data[E] = NewDataV.at(E);

	NewDataV.clear();

	// Update sv
	sv = std::string_view((char*)Data, Size);

	// Verbose output
	if (DEBUG)
		std::cout << "\n PrepModelForWrite: Model info is ready!\n";
}

// Make any needed adjustments/edits to vector before writing back to file
inline void MSH::PrepMSHForWrite()
{
	if (MSHChanged())
	{
		// Creates new MATL and MATD chunks for our material edits
		PrepMatForWrite();

		// Creates new MODL chunks for our model edits
		PrepModelForWrite();

		// Resize MSH2 and HEDR chunk headers
		MSH2_Position = GetChunk("MSH2") + 4;
		size_t CL1L_Pos = Size - 8;

		// Find end of MSH2 chunk and get the size
		if (BLN2_Position > 0)
			MSH2_Size = (BLN2_Position - (MSH2_Position + 4));
		else if (SKL2_Position > 0)
			MSH2_Size = (SKL2_Position - (MSH2_Position + 4));
		else if (ANM2_Position > 0)
			MSH2_Size = (ANM2_Position - (MSH2_Position + 4));
		else
			MSH2_Size = (CL1L_Pos - (MSH2_Position + 4));

		unsigned char MSH2Size[4] = { '\x00', '\x00', '\x00', '\x00' };

		// Write to a stringstream the MSH2 chunk size, then read it
		std::stringstream s;
		s.setf(std::ios::binary);
		s.write(reinterpret_cast<char*>(&MSH2_Size), 4);
		s.read(reinterpret_cast<char*>(&MSH2Size), 4);

		// Hold onto MSH2Size for when we overwrite the values after populating NewData

		HEDR_Size = CL1L_Pos;

		unsigned char HEDRSize[4] = { '\x00', '\x00', '\x00', '\x00' };

		// Write to a stringstream the HEDR chunk size, then read it
		std::stringstream t;
		t.setf(std::ios::binary);
		t.write(reinterpret_cast<char*>(&HEDR_Size), 4);
		t.read(reinterpret_cast<char*>(&HEDRSize), 4);

		// Ok now overwrite the HEDR size value
		for (unsigned short a = 0; a < 4; a++)
			Data[4 + static_cast<size_t>(a)] = HEDRSize[a];

		// Ok now overwrite the MSH2 size value
		for (unsigned short b = 0; b < 4; b++)
			Data[MSH2_Position + b] = MSH2Size[b];

		if (DEBUG)
			std::cout << "\n PrepMSHForWrite: Everything prepared to be written to file!\n";
	}
}

// Write the new MSH to file
inline bool MSH::WriteMSH()
{
	if (MSHChanged())
	{
		// Now try to do the actual saving
		std::ofstream OutFile(FileName.c_str(), std::ios::out | std::ios::binary);
		if (OutFile.is_open())
		{
			// Write file from data 
			OutFile.write((char*)Data, Size);

			// Close file and flush buffers
			OutFile.close();

			std::cout << "\n WriteMSH: MSH " << FileName << " Written!";

			return true;
		}
		else
			return false;
	}
	else
		return true;
}

// Returns whether or not the MSH has been altered and needs to be written to file
inline bool MSH::MSHChanged()
{
	for (unsigned short C = 0; C < ModelCount; C++)
		if (Models.at(C).MODLChanged)
			CHANGED = true;

	for (unsigned short D = 0; D < MaterialCount; D++)
		if (Materials.at(D).MATDChanged)
			CHANGED = true;

	return CHANGED;
}

// Imports a MODL chunk
inline bool MSH::ImportMODL()
{
	std::string MODLFileName;
	bool Valid_Name = false;
	do
	{
		// Prompt user for filename of msh
		std::cout << " Enter MODL chunk filename: ";

		// If input can be read, continue
		if (std::cin >> MODLFileName)
		{
			// Check if filename has .msh extension, if not add it
			if (!std::regex_match(MODLFileName, std::regex("(.*)(\\.msh)")))
				FileName = MODLFileName + ".modl";

			// Check if file exists
			std::ifstream TestFile(MODLFileName.c_str());
			if (TestFile.is_open())
			{
				Valid_Name = true;
				TestFile.close();
			}
			else
				std::cout << " Failed to open file! Please try again.\n\n";
		}
		else
			std::cout << " Error with input, please try again!\n\n";
	} while (!Valid_Name);

	// Open File
	std::ifstream InFile(MODLFileName.c_str(), std::ios::in | std::ios::binary | std::ios::ate);
	if (!InFile.is_open())
		return false;

	// Get the size of the file by recording stream position (at end by ios::ate)
	size_t MODLSize = static_cast<size_t>(InFile.tellg());

	// Allocate a new unsigned char array of the filesize to it
	unsigned char * MODLBuffer = new unsigned char[MODLSize];

	// Set the stream position to the beginning of the file
	InFile.seekg(InFile.beg);

	// Read file into the unsigned char array
	InFile.read((char*)MODLBuffer, MODLSize);

	// Close file and flush buffers
	InFile.close();

	// Ok now create a new Model object for it
	Model NewMODL;
	std::string_view MODLsv((char*)MODLBuffer, MODLSize);

	// Set insertion point right after last MODL chunk
	size_t InsertionPoint = Models.at(ModelCount - 1).MODL_Position + 4 + Models.at(ModelCount - 1).MODL_Size;
	NewMODL.MODL_Position = InsertionPoint + 4;
	size_t pos = NewMODL.MODL_Position;

	// Create substring of the MODL chunk size 
	std::string_view temp = MODLsv.substr(pos, 4);

	// Write to a std::string stream the MODL chunk size, then read it
	std::stringstream s;
	s.setf(std::ios::binary);
	s.write(temp.data(), 4);
	s.read(reinterpret_cast<char*>(&NewMODL.MODL_Size), 4);

	// Increment pos to skip MTYP header and size (always 4 bytes)
	pos += 12; // Now we're at MTYP
	NewMODL.MTYP_Position = pos + NewMODL.MODL_Position - 4;

	// Create substring of the MODL MTYP  
	std::string_view temp2 = MODLsv.substr(pos, 4);

	// Write to a std::string stream the MODL chunk size, then read it
	std::stringstream t;
	t.setf(std::ios::binary);
	t.write(temp2.data(), 4);
	t.read(reinterpret_cast<char*>(&NewMODL.MTYP), 4);

	// Increment pos to skip MNDX header and size (also always 4 bytes)
	pos += 12; // Now we're at MNDX
	NewMODL.MNDX_Position = pos + NewMODL.MODL_Position - 4;
	NewMODL.MNDX = ModelCount + 1;

	// Increment pos to go to Name chunk size
	pos += 8; // Now at Name size
	NewMODL.Name_Position = pos + NewMODL.MODL_Position - 4;

	// Create substring of the MODL Name size  
	std::string_view temp4 = MODLsv.substr(pos, 4);
	unsigned short NameLength = 0;

	// Write to a std::string stream the MODL MNDX
	std::stringstream v;
	v.setf(std::ios::binary);
	v.write(temp4.data(), 4);
	v.read(reinterpret_cast<char*>(&NameLength), 4);

	// Increment pos to start of name
	pos += 4;

	char* MODLName = new char[NameLength];

	// Now read the name from that length we got
	std::string_view temp5 = MODLsv.substr(pos, NameLength);

	std::stringstream w;
	w.setf(std::ios::binary);
	w.write(temp5.data(), NameLength);
	w.read(MODLName, NameLength);

	// Since we treated the string as an array we have to push back to do it "officially"
	std::string Name;
	for (unsigned short S = 0; S < NameLength; S++)
		Name.push_back(MODLName[S]);

	delete[] MODLName;

	NewMODL.Name = Name;
	NewMODL.Name_Size = NameLength;
	NewMODL.OG_Value[1] = NameLength;

	// Increment pos the length of the name
	pos += NameLength;

	pos = MODLsv.find("PRNT");
	if (pos < MODLSize)
	{
		pos += 4; // Skip to name size
		NewMODL.PRNT_Position = pos + NewMODL.MODL_Position - 4;
		NewMODL.PRNT_Index = 1;
		
		// Create substd::string of the model name size 
		std::string_view prntsv = MODLsv.substr(pos, 4);
		unsigned short NameLen = 0;

		// Write to a std::string stream the model name size, then read it to ModelNameLength
		std::stringstream s;
		s.setf(std::ios::binary);
		s.write(prntsv.data(), 4);
		s.read(reinterpret_cast<char*>(&NameLen), 4);
		
		pos += NameLen + 4;

		NewMODL.OG_Value[0] = NameLen;
		NewMODL.CHANGED[0] = true;
		NewMODL.PRNT = Models.at(0).Name;
		NewMODL.PRNT_Size = Models.at(0).Name_Size;
	}
	else
	{
		NewMODL.OG_Value[0] = 0;
		NewMODL.CHANGED[0] = true;
		NewMODL.PRNT = Models.at(0).Name;
		NewMODL.PRNT_Size = Models.at(0).Name_Size;
		pos = 8;
	}

	pos = MODLsv.find("FLGS");
	if (pos < MODLSize)
	{
		// Skip header and go to FLGS
		pos += 8;

		// Record FLGS 
		NewMODL.FLGS_Position = pos + NewMODL.MODL_Position - 4;
		NewMODL.FLGS = bool(MODLsv.at(pos));
		NewMODL.OG_Value[2] = MODLsv.at(pos);
		NewMODL.CHANGED[2] = true;
	}
	else
		pos = 8;

	pos = MODLsv.find("GEOM");
	if (pos < MODLSize)
	{
		// Skip header and record position at GEOM size
		pos += 4;
		NewMODL.GEOM_Position = pos + NewMODL.MODL_Position - 4;

		// Create substring of the GEOM chunk size 
		std::string_view geomsv = MODLsv.substr(pos, 4);

		// Write to a string stream the GEOM size
		std::stringstream x;
		x.setf(std::ios::binary);
		x.write(geomsv.data(), 4);
		x.read(reinterpret_cast<char*>(&NewMODL.GEOM_Size), 4);
		pos += 4;
	}
	else
		pos = 8;

	pos = MODLsv.find("CLTH");
	if (pos < MODLSize)
	{
		// Skip header and record position at CLTH size
		pos += 4;
		NewMODL.CLTH_Position = pos + NewMODL.MODL_Position - 4;
		NewMODL.CLTH = true;

		// Create substring of the Cloth chunk size 
		std::string_view tempsv = MODLsv.substr(pos, 4);

		// Write to a string stream CLTH chunk size
		std::stringstream s;
		s.setf(std::ios::binary);
		s.write(tempsv.data(), 4);
		s.read(reinterpret_cast<char*>(&NewMODL.CLTH_Size), 4);

		pos += 8; // Skip past CLTH size and CTEX header to CTEX name size position

		NewMODL.CTEX_Position = pos + NewMODL.MODL_Position - 4;

		// Create substring of the Cloth texture name size 
		std::string_view tempsv3 = MODLsv.substr(pos, 4);
		unsigned short NameLen = 0;

		// Write to a std::string stream the model name size, then read it to NameLen
		std::stringstream u;
		u.setf(std::ios::binary);
		u.write(tempsv3.data(), 4);
		u.read(reinterpret_cast<char*>(&NameLen), 4);

		// Update pos to stay synced
		pos += 4;

		char* Tex_Name = new char[NameLen];

		// Now read from that length we got and record it
		std::string_view tempsv2 = MODLsv.substr(pos, NameLen);
		std::stringstream t;
		t.setf(std::ios::binary);
		t.write(tempsv2.data(), NameLen);
		t.read(Tex_Name, NameLen);

		std::string Name;

		for (unsigned short S = 0; S < NameLen; S++)
			Name.push_back(Tex_Name[S]);

		delete[] Tex_Name;

		NewMODL.CTEX = Name;
		NewMODL.CTEX_Size = NameLen;
		NewMODL.OG_Value[3] = NameLen;
	}
	else
		NewMODL.CLTH = false;

	pos = 8;
	pos = MODLsv.find("SEGM", pos);
	while (pos < MODLSize)
	{
		// Keep doing this while still before next MODL chunk
		Segment NewSEGM;

		// Skip to size and record position to Segment
		pos += 4;
		NewSEGM.SEGM_Position = pos;

		// Record size
		std::string_view temp = MODLsv.substr(pos, 4);

		// Write to a std::string stream the SEGM size, then record it
		std::stringstream y;
		y.setf(std::ios::binary);
		y.write(temp.data(), 4);
		y.read(reinterpret_cast<char*>(&NewSEGM.SEGM_Size), 4);

		size_t matipos = GetChunk("MATI", pos);
		matipos += 8; // Skip to MATI value
		NewSEGM.MATI_Position = matipos;

		// Create substring of the MATI
		std::string_view temp3 = MODLsv.substr(matipos, 4);

		// Write to a std::string stream the MATI, then record it
		std::stringstream z;
		z.setf(std::ios::binary);
		z.write(temp3.data(), 4);
		z.read(reinterpret_cast<char*>(&NewSEGM.MATI), 4);

		if (NewSEGM.MATI >= MaterialCount)
		{
			NewSEGM.MATI = 0;
			NewMODL.CHANGED[4] = true;
		}

		// Push the segment to this MODL
		NewMODL.Segments.push_back(NewSEGM);
		pos = MODLsv.find("SEGM", pos);
	}

	// Split Data in two before and after insertion point
	std::vector<unsigned char> DataBufferBefore;
	std::vector<unsigned char> DataBufferAfter;
	DataBufferBefore.reserve(Size);
	DataBufferAfter.reserve(Size);

	for (size_t N = 0; N < InsertionPoint; N++)
		DataBufferBefore.push_back(sv.at(N));

	for (size_t N = InsertionPoint; N < Size; N++)
		DataBufferAfter.push_back(sv.at(N));

	delete[] Data;
	Size = Size + MODLSize;
	Data = new unsigned char[Size];

	for (size_t N = 0; N < DataBufferBefore.size(); N++)
		Data[N] = DataBufferBefore.at(N);

	for (size_t N = 0; N < MODLSize; N++)
		Data[InsertionPoint + N] = MODLBuffer[N];

	for (size_t N = 0; N < DataBufferAfter.size(); N++)
		Data[InsertionPoint + MODLSize + N] = DataBufferAfter.at(N);

	delete[] MODLBuffer;
	DataBufferBefore.clear();
	DataBufferAfter.clear();
	Models.push_back(NewMODL);
	ModelCount++;
	sv = std::string_view((char*)Data);
	NewMODL.CHANGED = true;
	CHANGED = true;

	return true;
}

// Renames the selected material
inline void MSH::RenameMaterial(unsigned short Selected, std::string name)
{
	std::string NewName = name;
	if (NewName.length() > 0)
	{
		// Remove any spaces
		for (unsigned short ch = 0; ch < NewName.length(); ch++)
			if (std::isspace(NewName.at(ch)) != 0)
				NewName.at(ch) = '_';

		std::vector<unsigned char> NewNameV;
		NewNameV.reserve(NewName.length());
		for (unsigned char hc : NewName)
			NewNameV.push_back(hc);

		PadString(NewNameV);
		NewName.clear();
		for (unsigned char mc : NewNameV)
			NewName.push_back(mc);

		Materials.at(Selected).MatName = NewName;
		Materials.at(Selected).MatName_Size = static_cast<uint32_t>(NewNameV.size());
		Materials.at(Selected).MATDChanged = true;
	}
}

// Sets or toggles a selected flag from the specified material
inline void MSH::SetFlag(unsigned short Selected, unsigned short Flag, unsigned short Value = 0)
{
	if (Flag > 0 && Flag < 9)
	{
		if (Value == 1)
			std::get<1>(Materials.at(Selected).MatFlags[Flag - 1]) = true;
		else if (Value == 2)
			std::get<1>(Materials.at(Selected).MatFlags[Flag - 1]) = false;
		else
		{
			if (std::get<1>(Materials.at(Selected).MatFlags[Flag - 1]))
				std::get<1>(Materials.at(Selected).MatFlags[Flag - 1]) = false;
			else if (!std::get<1>(Materials.at(Selected).MatFlags[Flag - 1]))
				std::get<1>(Materials.at(Selected).MatFlags[Flag - 1]) = true;
		}
		Materials.at(Selected).MATDChanged = true;
	}
}

// Sets the RenderType of the selected material
inline void MSH::SetRT(unsigned short Selected, unsigned short RT)
{
	Materials.at(Selected).RenderType = static_cast<unsigned char>(RT);
	Materials.at(Selected).MATDChanged = true;
}

// Sets Data0 value of selected material
inline void MSH::SetData0(unsigned short Selected, unsigned short V)
{
	unsigned short NewV = V;

	if (NewV >= 0 && NewV <= 255)
		Materials.at(Selected).Data0 = static_cast<unsigned char>(NewV);

	Materials.at(Selected).MATDChanged = true;
}

// Sets Data1 value of selected material
inline void MSH::SetData1(unsigned short Selected, unsigned short V)
{
	unsigned short NewV = V;

	if (NewV >= 0 && NewV <= 255)
		Materials.at(Selected).Data1 = static_cast<unsigned char>(NewV);

	Materials.at(Selected).MATDChanged = true;
}

// Sets texture name of TX0D
inline void MSH::SetTX0D(unsigned short Selected, std::string TexName)
{
	std::string NewName = TexName;
	// Remove any spaces
	for (unsigned short ch = 0; ch < NewName.length(); ch++)
		if (std::isspace(NewName.at(ch)) != 0)
			NewName.at(ch) = '_';

	std::vector<unsigned char> NewNameV;
	NewNameV.reserve(NewName.length());
	for (unsigned char hc : NewName)
		NewNameV.push_back(hc);

	// Pad string with NULLs
	PadString(NewNameV);
	NewName.clear();

	// Push back to string
	for (unsigned char mc : NewNameV)
		NewName.push_back(mc);

	Materials.at(Selected).TX0D = NewName;
	Materials.at(Selected).TX0D_Size = static_cast<uint32_t>(NewNameV.size());
	Materials.at(Selected).MATDChanged = true;
}

// Sets texture name of TX1D
inline void MSH::SetTX1D(unsigned short Selected, std::string TexName)
{
	std::string NewName = TexName;
	// Remove any spaces
	for (unsigned short ch = 0; ch < NewName.length(); ch++)
		if (std::isspace(NewName.at(ch)) != 0)
			NewName.at(ch) = '_';

	std::vector<unsigned char> NewNameV;
	NewNameV.reserve(NewName.length());
	for (unsigned char hc : NewName)
		NewNameV.push_back(hc);

	// Pad string with NULLs
	PadString(NewNameV);
	NewName.clear();

	// Push back to string
	for (unsigned char mc : NewNameV)
		NewName.push_back(mc);

	Materials.at(Selected).TX1D = NewName;
	Materials.at(Selected).TX1D_Size = static_cast<uint32_t>(NewNameV.size());
	Materials.at(Selected).MATDChanged = true;
}

// Sets texture name of TX2D
inline void MSH::SetTX2D(unsigned short Selected, std::string TexName)
{
	std::string NewName = TexName;
	// Remove any spaces
	for (unsigned short ch = 0; ch < NewName.length(); ch++)
		if (std::isspace(NewName.at(ch)) != 0)
			NewName.at(ch) = '_';

	std::vector<unsigned char> NewNameV;
	NewNameV.reserve(NewName.length());
	for (unsigned char hc : NewName)
		NewNameV.push_back(hc);

	// Pad string with NULLs
	PadString(NewNameV);
	NewName.clear();

	// Push back to string
	for (unsigned char mc : NewNameV)
		NewName.push_back(mc);

	Materials.at(Selected).TX2D = NewName;
	Materials.at(Selected).TX2D_Size = static_cast<uint32_t>(NewNameV.size());
	Materials.at(Selected).MATDChanged = true;
}

// Sets texture name of TX3D
inline void MSH::SetTX3D(unsigned short Selected, std::string TexName)
{
	std::string NewName = TexName;
	// Remove any spaces
	for (unsigned short ch = 0; ch < NewName.length(); ch++)
		if (std::isspace(NewName.at(ch)) != 0)
			NewName.at(ch) = '_';

	std::vector<unsigned char> NewNameV;
	NewNameV.reserve(NewName.length());
	for (unsigned char hc : NewName)
		NewNameV.push_back(hc);

	// Pad string with NULLs
	PadString(NewNameV);
	NewName.clear();

	// Push back to string
	for (unsigned char mc : NewNameV)
		NewName.push_back(mc);

	Materials.at(Selected).TX3D = NewName;
	Materials.at(Selected).TX3D_Size = static_cast<uint32_t>(NewNameV.size());
	Materials.at(Selected).MATDChanged = true;
}

// Set BGRA value for diffuse 
inline void MSH::SetDiffuseGBRA(unsigned short Selected, float RGBA[4])
{
	float R = RGBA[0];
	float G = RGBA[1];
	float B = RGBA[2];
	float A = RGBA[3];

	if (R >= 0.0 && R <= 1.0 && G >= 0.0 && G <= 1.0 && B >= 0.0 && B <= 1.0 && A >= 0.0 && A <= 1.0)
	{
		Materials.at(Selected).D_RGBA[0] = B;
		Materials.at(Selected).D_RGBA[1] = G;
		Materials.at(Selected).D_RGBA[2] = R;
		Materials.at(Selected).D_RGBA[3] = A;
		Materials.at(Selected).MATDChanged = true;
	}
	else
		std::cout << "\n The values must be between 0.0 and 1.0!\n";
}

// Set BGRA value for diffuse 
inline void MSH::SetAmbientGBRA(unsigned short Selected, float RGBA[4])
{
	float R = RGBA[0];
	float G = RGBA[1];
	float B = RGBA[2];
	float A = RGBA[3];

	if (R >= 0.0 && R <= 1.0 && G >= 0.0 && G <= 1.0 && B >= 0.0 && B <= 1.0 && A >= 0.0 && A <= 1.0)
	{
		Materials.at(Selected).A_RGBA[0] = B;
		Materials.at(Selected).A_RGBA[1] = G;
		Materials.at(Selected).A_RGBA[2] = R;
		Materials.at(Selected).A_RGBA[3] = A;
		Materials.at(Selected).MATDChanged = true;
	}
	else
		std::cout << "\n The values must be between 0.0 and 1.0!\n";
}

// Set BGRA value for diffuse 
inline void MSH::SetSpecularGBRA(unsigned short Selected, float RGBA[4])
{
	float R = RGBA[0];
	float G = RGBA[1];
	float B = RGBA[2];
	float A = RGBA[3];

	if (R >= 0.0 && R <= 1.0 && G >= 0.0 && G <= 1.0 && B >= 0.0 && B <= 1.0 && A >= 0.0 && A <= 1.0)
	{
		Materials.at(Selected).S_RGBA[0] = B;
		Materials.at(Selected).S_RGBA[1] = G;
		Materials.at(Selected).S_RGBA[2] = R;
		Materials.at(Selected).S_RGBA[3] = A;
		Materials.at(Selected).MATDChanged = true;
	}
	else
		std::cout << "\n The values must be between 0.0 and 1.0!\n";
}

// Set BGRA value for diffuse 
inline void MSH::SetSpecularDecay(unsigned short Selected, unsigned int Value)
{
	Materials.at(Selected).S_Decay = static_cast<float>(Value);
	Materials.at(Selected).MATDChanged = true;
}

// Renames the selected model
inline void MSH::RenameModel(unsigned short Selected, std::string name)
{
	std::string NewName = name;
	if (NewName.length() > 0)
	{
		// Remove any spaces
		for (unsigned short ch = 0; ch < NewName.length(); ch++)
			if (std::isspace(NewName.at(ch)) != 0)
				NewName.at(ch) = '_';

		std::vector<unsigned char> NewNameV;
		NewNameV.reserve(NewName.length());
		for (unsigned char hc : NewName)
			NewNameV.push_back(hc);

		PadString(NewNameV);
		NewName.clear();
		for (unsigned char mc : NewNameV)
			NewName.push_back(mc);

		// Change PRNT name of all children to new name
		for (unsigned short D = 0; D < ModelCount; D++)
			if (Models.at(D).PRNT_Index == Models.at(Selected).MNDX)
			{
				Models.at(D).PRNT = NewName;
				Models.at(D).PRNT_Size = static_cast<uint32_t>(NewNameV.size());
			}

		// Now actually do it
		if (!Models.at(Selected).CHANGED[1])
			Models.at(Selected).OG_Value[1] = Models.at(Selected).Name_Size;

		Models.at(Selected).Name = NewName;
		Models.at(Selected).Name_Size = static_cast<uint32_t>(NewNameV.size());
		Models.at(Selected).MODLChanged = true;
		Models.at(Selected).CHANGED[1] = true;
	}
}

// Sets the vertex color CLRB chunk
inline void MSH::SetCLRB(unsigned short Selected, unsigned short Cluster, unsigned short RGBA[4])
{
	unsigned short R = RGBA[0];
	unsigned short G = RGBA[1];
	unsigned short B = RGBA[2];
	unsigned short A = RGBA[3];

	if (R >= 0 && R <= 255 && G >= 0 && G <= 255 && B >= 0 && B <= 255 && A >= 0 && A <= 255)
	{
		Models.at(Selected).Segments.at(Cluster).CLRB[0] = B;
		Models.at(Selected).Segments.at(Cluster).CLRB[1] = G;
		Models.at(Selected).Segments.at(Cluster).CLRB[2] = R;
		Models.at(Selected).Segments.at(Cluster).CLRB[3] = A;
		Models.at(Selected).Segments.at(Cluster).CLRB_Present = true;
		Models.at(Selected).CHANGED[5] = true;
		Models.at(Selected).MODLChanged = true;
	}
	else
		std::cout << "\n The values must be between 0 and 255!\n";
}

// Removes vertex colors from the modl
inline void MSH::RemoveColors(unsigned short Selected)
{
	for (unsigned short C = 0; C < Models.at(Selected).Segments.size(); C++)
	{
		Models.at(Selected).Segments.at(C).CLRB[0] = 0;
		Models.at(Selected).Segments.at(C).CLRB[1] = 0;
		Models.at(Selected).Segments.at(C).CLRB[2] = 0;
		Models.at(Selected).Segments.at(C).CLRB[3] = 0;
		Models.at(Selected).Segments.at(C).CLRB_Present = false;
		Models.at(Selected).Segments.at(C).CLRL_Count = 0;
		Models.at(Selected).Segments.at(C).CLRL.clear();
		Models.at(Selected).Segments.at(C).CLRL_Present = false;
		Models.at(Selected).MODLChanged = true;
		Models.at(Selected).CHANGED[5] = true;
		Models.at(Selected).CHANGED[6] = true;
	}
}

// Sets the parent of the selected modl
inline void MSH::SetModelParent(unsigned short Selected, unsigned short ModelIndex)
{
	unsigned short NewPRNT = ModelIndex;

	if (NewPRNT > 0 && NewPRNT <= ModelCount)
		if (NewPRNT != Selected && NewPRNT != Models.at(NewPRNT).PRNT_Index - 1)
		{
			if (!Models.at(Selected).CHANGED[0])
				Models.at(Selected).OG_Value[0] = Models.at(Selected).PRNT_Size;

			Models.at(Selected).PRNT = Models.at(NewPRNT).Name;
			Models.at(Selected).PRNT_Size = Models.at(NewPRNT).Name_Size;
			Models.at(Selected).PRNT_Index = NewPRNT + 1;
			Models.at(Selected).MODLChanged = true;
			Models.at(Selected).CHANGED[0] = true;
		}
}

// Sets the visibility of the selected modl
inline void MSH::SetModelVisibility(unsigned short Selected, unsigned short Visible)
{
	bool Value;
	if (Visible == 1)
		Value = true;
	else
		Value = false;

	// Make Visible or Hidden
	if (Value && Models.at(Selected).FLGS == true)
	{
		// Now actually do it
		if (!Models.at(Selected).CHANGED[2])
			Models.at(Selected).OG_Value[2] = unsigned short(Models.at(Selected).FLGS);

		Models.at(Selected).FLGS = false;
		Models.at(Selected).MODLChanged = true;
		Models.at(Selected).CHANGED[2] = true;
	}
	else if (!Value && Models.at(Selected).FLGS == false)
	{
		// Now actually do it
		if (!Models.at(Selected).CHANGED[2])
			Models.at(Selected).OG_Value[2] = unsigned short(Models.at(Selected).FLGS);

		Models.at(Selected).FLGS = true;
		Models.at(Selected).MODLChanged = true;
		Models.at(Selected).CHANGED[2] = true;
	}
}

// Sets the MATI of the selected SEGM
inline void MSH::SetClusterMaterial(unsigned short modl, unsigned short cluster, unsigned short material)
{
	// Check that the indices are all valid
	if (modl >= 1 && modl < ModelCount)
		if (cluster >= 0 && cluster < Models.at(modl).Segments.size())
			if (material >= 0 && material < MaterialCount)
			{
				Models.at(modl).Segments.at(cluster).MATI = material;
				Models.at(modl).MODLChanged = true;
				Models.at(modl).CHANGED[4] = true;
			}
}

// Sets the name of the CTEX
inline void MSH::SetClothTex(unsigned short modl, std::string TexName)
{
	std::string NewName = TexName;
	if (TexName.length() > 0 && Models.at(modl).CLTH)
	{
		// Remove any spaces
		for (unsigned short ch = 0; ch < NewName.length(); ch++)
			if (std::isspace(NewName.at(ch)) != 0)
				NewName.at(ch) = '_';

		std::vector<unsigned char> NewNameV;
		NewNameV.reserve(NewName.length());
		for (unsigned char hc : NewName)
			NewNameV.push_back(hc);

		PadString(NewNameV);
		NewName.clear();
		for (unsigned char mc : NewNameV)
			NewName.push_back(mc);

		// Now actually do it
		if (!Models.at(modl).CHANGED[3])
			Models.at(modl).OG_Value[3] = Models.at(modl).CTEX_Size;

		Models.at(modl).CTEX = NewName;
		Models.at(modl).CTEX_Size = static_cast<unsigned short>(NewNameV.size());
		Models.at(modl).MODLChanged = true;
		Models.at(modl).CHANGED[3] = true;
	}
}

// Displays all models according to criteria
inline void MSH::ListModels()
{
	std::cout << " Model Info displayed as follows: \n MODEL INDEX, NAME, ASSIGNED MATERIAL, VISIBILITY, PARENT \n\n";
	std::regex norm("(p_)(.*)|(collision)(.*)|(sv_)(.*)|(shadowvolume)(.*)|(c_)(.*)|(eff_)(.*)|(root_)(.*)|(bone_)(.*)|(hp_)(.*)");
	for (auto m : Models)
	{
		if (ADVANCEDMODELS)
		{
			std::cout << ' ' << m.MNDX << " | " << ' ' << m.Name << "  |  ";
			if (m.CLTH)
				std::cout << m.CTEX << "  |  ";
			else
			{
				if (m.Segments.size() > 0)
					std::cout << Materials.at(m.Segments.at(0).MATI).MatName << "  |  ";
				else
					std::cout << "None" << " | ";
			}

			if (m.FLGS)
				std::cout << "Invisible | ";
			else
				std::cout << "Visible | ";

			if (!m.PRNT.empty())
				std::cout << m.PRNT;
			else
				std::cout << "None";

			std::cout << "\n\n";
		}
		else if (!std::regex_match(m.Name, norm) && m.MTYP != 0 && m.MTYP != 3)

		{
			std::cout << ' ' << m.MNDX << " | " << ' ' << m.Name << "  |  ";
			if (m.CLTH)
				std::cout << m.CTEX << "  |  ";
			else
			{
				if (m.Segments.size() > 0)
					std::cout << Materials.at(m.Segments.at(0).MATI).MatName << "  |  ";
				else
					std::cout << "None" << " | ";
			}
			if (m.FLGS)
				std::cout << "Invisible | ";
			else
				std::cout << "Visible | ";

			if (!m.PRNT.empty())
				std::cout << m.PRNT;
			else
				std::cout << "None";

			std::cout << "\n\n";
		}
	}
}

// Displays all materials according to criteria
inline void MSH::ListMaterials()
{
	std::cout << " Material Info displayed as follows: \n MATERIAL INDEX, NAME, RENDERTYPE, MATERIAL-FLAGS, TEXTURES \n\n";
	for (auto m : Materials)
	{
		std::cout << ' ' << m.MATI << " | " << ' ' << m.MatName << "  |  " << int(m.RenderType) << "  |  ";

		bool HasFlag = false;
		for (short d = 0; d < 8; d++)
		{
			if (std::get<1>(m.MatFlags[d]))
			{
				std::cout << std::get<0>(m.MatFlags[d]) << ' ';
				HasFlag = true;
			}
		}
		if (!HasFlag)
			std::cout << "None";

		std::cout << "  |  ";

		if (!m.TX0D.empty())
			std::cout << " TX0D: " << m.TX0D << ' ';
		else
			std::cout << " TX0D: NO TEXTURE SET";
		if (!m.TX1D.empty())
			std::cout << " \n TX1D: " << m.TX1D << ' ';
		if (!m.TX2D.empty())
			std::cout << " \n TX2D: " << m.TX2D << ' ';
		if (!m.TX3D.empty())
			std::cout << " \n TX3D: " << m.TX3D << ' ';

		std::cout << "\n\n";
	}
}