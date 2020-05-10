#pragma once

// Helper function to recieve commands from user
template <typename T>
T GetUserInput(std::string OptionalMessage = "")
{
    // What will be received by the user and returned
    T Input;

    // Loop to ensure valid and correct input
    // Sentinel
    bool Valid_Input = false;
    do
    {
        // Prompt user (if specified)
        std::cout << OptionalMessage << ' ';

        // If input can be read, continue
        if (std::cin >> Input)
            Valid_Input = true;
        else
        {
            std::cin.clear();
            std::cin.ignore();
            std::cout << " Error with input, please try again!\n\n";
        }

    } while (!Valid_Input);

    return Input;
}

// Static functions that push and pop requested screens
class View
{
public:

    // Helper function to get BGRA values from user
    static inline void SetRGBA(MSH& MSHFile, unsigned short Selected, unsigned char type, unsigned short Cluster = 0)
    {
        if (type == 'D' || type == 'S' || type == 'A')
        {
            float R = GetUserInput<float>("\n Enter RED value from 0.0 - 1.0:");
            float G = GetUserInput<float>("\n Enter GREEN value from 0.0 - 1.0:");
            float B = GetUserInput<float>("\n Enter BLUE value from 0.0 - 1.0:");
            float A = GetUserInput<float>("\n Enter ALPHA value from 0.0 - 1.0:");
            float RGBA[4] = { R, G, B, A };

            if (type == 'D')
                MSHFile.SetDiffuseGBRA(Selected, RGBA);
            else if (type == 'S')
                MSHFile.SetSpecularGBRA(Selected, RGBA);
            else if (type == 'A')
                MSHFile.SetAmbientGBRA(Selected, RGBA);

            MSHFile.Materials.at(Selected).MATDChanged = true;
        }
        else
        {
            if (type == 'E')
            {
                std::cout << "\n Current BGRA values are: ";
                for (unsigned short C = 0; C < 4; C++)
                    std::cout << (unsigned short)MSHFile.Models.at(Selected).Segments.at(Cluster).CLRB[C] << ' ';
                std::cout << "\n ";
            }

            unsigned short R = GetUserInput<unsigned short>("\n Enter RED value from 0 - 255:");
            unsigned short G = GetUserInput<unsigned short>("\n Enter GREEN value from 0 - 255:");
            unsigned short B = GetUserInput<unsigned short>("\n Enter BLUE value from 0 - 255:");
            unsigned short A = GetUserInput<unsigned short>("\n Enter ALPHA value from 0 - 255:");
            unsigned short RGBA[4] = { R, G, B, A };

            MSHFile.SetCLRB(Selected, Cluster, RGBA);

            MSHFile.Models.at(Selected).MODLChanged = true;
        }
    }

    // Screen where advanced options are displayed for material editing
    static inline bool MaterialEditorAdvanced(MSH& MSHFile, unsigned short Selected)
    {
        std::cout << "\n Diffuse BGRA: ";
        for (short f = 0; f < 4; f++)
            std::cout << std::fixed << std::setprecision(2) << MSHFile.Materials.at(Selected).D_RGBA[f] << ' ';

        std::cout << "\t Specular BGRA: ";
        for (short g = 0; g < 4; g++)
            std::cout << std::fixed << std::setprecision(2) << MSHFile.Materials.at(Selected).S_RGBA[g] << ' ';

        std::cout << "\t Ambient BGRA: ";
        for (short h = 0; h < 4; h++)
            std::cout << std::fixed << std::setprecision(2) << MSHFile.Materials.at(Selected).A_RGBA[h] << ' ';

        std::cout << "\n Specular Decay: " << std::fixed << std::setprecision(0) << (MSHFile.Materials.at(Selected).S_Decay) << '\n';

        std::cout << "\n\n Select property to edit (ENTER NUMBER)"
            "\n 1. Diffuse BGRA   2. Specular BGRA   3. Ambient BGRA   4. Specular Decay   5. Help   6. Go Back";

        unsigned short option = GetUserInput<unsigned short>("\n");

        if (option == 1)
            SetRGBA(MSHFile, Selected, 'D');
        else if (option == 2)
            SetRGBA(MSHFile, Selected, 'S');
        else if (option == 3)
            SetRGBA(MSHFile, Selected, 'A');
        else if (option == 4)
        {
            unsigned int Decay = GetUserInput<unsigned int>("Enter Specular Decay value:");
            MSHFile.SetSpecularDecay(Selected, Decay);
        }
        else if (option == 5)
            HelpView(1);
        else if (option == 6)
            return true;

        return false;
    }

    // Screen where materials can be added
    static inline void MaterialAdder(MSH& MSHFile)
    {
        // Create new Material object and fill basic info
        Material NewMat;
        std::string NewMatName;
        bool Valid = false;
        do
        {
            std::cout << "\n Enter the name of your new material: ";
            std::cin.ignore();
            std::getline(std::cin, NewMatName, '\n');

            // If input can be read and is valid, continue
            if (!std::cin.bad() && NewMatName.length() > 0)
                Valid = true;
            else
            {
                std::cin.clear();
                std::cin.ignore();
                std::cout << " Error with input, please try again!\n\n";
            }

        } while (!Valid);

        // Remove any spaces
        for (unsigned short ch = 0; ch < NewMatName.length(); ch++)
            if (std::isspace(NewMatName.at(ch)) != 0)
                NewMatName.at(ch) = '_';

        std::vector<unsigned char> NewNameV;
        NewNameV.reserve(NewMatName.length());
        for (unsigned char hc : NewMatName)
            NewNameV.push_back(hc);

        MSHFile.PadString(NewNameV);
        NewMatName.clear();
        for (unsigned char mc : NewNameV)
            NewMatName.push_back(mc);

        NewMat.MatName = NewMatName;
        NewMat.MatName_Size = static_cast<unsigned short>(NewNameV.size());

        std::string NewTexName;
        bool Valid2 = false;
        do
        {
            std::cout << "\n Now enter the texture name for the diffuse texture of this material: ";
            //std::cin.ignore();
            std::getline(std::cin, NewTexName, '\n');

            // If input can be read and is valid, continue
            if (!std::cin.bad())
                Valid2 = true;
            else
            {
                std::cin.clear();
                std::cin.ignore();
                std::cout << " Error with input, please try again!\n\n";
            }

        } while (!Valid2);

        // Remove any spaces
        for (unsigned short ch = 0; ch < NewTexName.length(); ch++)
            if (std::isspace(NewTexName.at(ch)) != 0)
                NewTexName.at(ch) = '_';

        std::vector<unsigned char> NewTNameV;
        NewTNameV.reserve(NewTexName.length());
        for (unsigned char hc : NewTexName)
            NewTNameV.push_back(hc);

        MSHFile.PadString(NewTNameV);
        NewTexName.clear();
        for (unsigned char mc : NewTNameV)
            NewTexName.push_back(mc);

        // Save changes to Material and push material to main vector
        NewMat.TX0D = NewTexName;
        NewMat.TX0D_Size = static_cast<unsigned short>(NewTNameV.size());
        NewMat.MATI = MSHFile.MaterialCount;
        NewMat.MATDChanged = true;
        MSHFile.MaterialCount++;
        MSHFile.Materials.push_back(NewMat);

        std::cout << "\n New material successfully added! You can edit it further from the Edit a Material screen.\n ";
    }

    // Screen where the selected material can be edited by the user
    static inline bool MaterialEditor(MSH &MSHFile, unsigned short Selected)
    {
        // Prompt user for action and get input
        std::cout << "\n\n Select property to edit (ENTER NUMBER)"
            "\n 1. Name   2. RenderType   3. Flags   4. Textures   5. Data0   6. Data1   7. Advanced   8. Help   9. Go Back";

        unsigned short ToEdit = GetUserInput<unsigned short>("\n");

        // Take the specified action
        if (ToEdit == 1)
        {
            std::string NewName;
            bool Valid = false;
            std::cin.ignore();

            do
            {
                std::cout << "\n Enter new material name: ";
                std::getline(std::cin, NewName, '\n');

                // If input can be read and is valid, continue
                if (!std::cin.bad() && NewName.length() > 0)
                    Valid = true;
                else
                {
                    std::cin.clear();
                    std::cin.ignore();
                    std::cout << " Error with input, please try again!\n\n";
                }

            } while (!Valid);

            std::cout << ' ' << MSHFile.Materials.at(Selected).MatName
                << " renamed to " << NewName << '\n';

            MSHFile.RenameMaterial(Selected, NewName);
        }
        else if (ToEdit == 2)
        {
            std::cout << "\n RenderType options are:"
                "\n 0 -> Normal | 1 -> Glow (unused?) | 2 -> Lightmap | 3 -> Scrolling | 4 -> Specular | 5 -> GlossMap"
                "\n 6 -> Chrome (envmap) | 7 -> Animated | 8 -> Ice (unused?) | 9 -> Sky (unused?) | 10 -> Water (unused?)"
                "\n 11 -> Detail | 12 -> 2 Scroll V (?) | 13 -> Rotate | 14 -> Glow Rotate | 15 -> Planar Reflection (unused?)"
                "\n 16 -> Glow Scroll | 17 -> Glow 2 Scroll (?) | 18 -> Curved Reflection (unused?) | 19 -> NormalMap Fade (?)"
                "\n 20 -> NormalMap InvFade (?) | 21 -> Ice Reflection (?) | 22 -> Ice Refraction | 23 -> Emboss"
                "\n 24 -> Wireframe (?) | 25 -> Energy | 26 Afterburner (?) | 27 -> BumpMap | 28 -> BumpMap+GlossMap"
                " \n 29 -> Teleportal (?) | 30 -> MultiState (?) | 31 -> Shield (?)";

            short NewRT = GetUserInput<short>("\n Enter new RenderType number:");
            std::cout << "\n Material " << MSHFile.Materials.at(Selected).MatName
                << " RenderType changed from " << short(MSHFile.Materials.at(Selected).RenderType) 
                << " to " << short(NewRT);

            // Now actually do it
            MSHFile.SetRT(Selected, NewRT);
        }
        else if (ToEdit == 3)
        {
            // Prompt and get flag, then value
            std::cout << "\n Flag numbers are:"
                "\n 1. Specular   2. Additive   3. PerPixel   4. Hard   5. Double   6. Single   7. Glow   8. Emissive   9. Go Back";
            short Flag = GetUserInput<short>("\n Enter flag number to edit:");
            if (Flag > 0 && Flag < 9)
            {
                short Value = GetUserInput<short>("\n Choose Value (ENTER NUMBER)\n 1. True   2. False \n");
                MSHFile.SetFlag(Selected, Flag, Value);
            }
        }
        else if (ToEdit == 4)
        {
            // Prompt user to choose to rename a texture chunk
            std::cout << "\n Select a texture chunk to rename: "
                "\n 1. TX0D   2. TX1D   3. TX2D   4. TX3D   5. Go Back\n";

            short chosentex = GetUserInput<short>();
            if (chosentex == 1 || chosentex == 2 || chosentex == 3 || chosentex == 4)
            {
                std::string NewName;
                bool Valid = false;

                do
                {
                    std::cout << "\n Enter new texture name for ";
                    if (chosentex == 1)
                        std::cout << "TX0D: ";
                    else if (chosentex == 2)
                        std::cout << "TX1D: ";
                    else if (chosentex == 3)
                        std::cout << "TX2D: ";
                    else if (chosentex == 4)
                        std::cout << "TX3D: ";

                    std::cin.ignore();
                    std::getline(std::cin, NewName, '\n');

                    // If input can be read and is valid, continue
                    if (!std::cin.bad())
                        Valid = true;
                    else
                    {
                        std::cin.clear();
                        std::cin.ignore();
                        std::cout << " Error with input, please try again!\n\n";
                    }

                } while (!Valid);

                if (chosentex == 1)
                {
                    std::cout << "\n TX0D " << MSHFile.Materials.at(Selected).TX0D
                        << " renamed to " << NewName << '\n';

                    // Now actually do it
                    MSHFile.SetTX0D(Selected, NewName);
                }
                else if (chosentex == 2)
                {
                    std::cout << "\n TX1D " << MSHFile.Materials.at(Selected).TX1D
                        << " renamed to " << NewName << '\n';

                    // Now actually do it
                    MSHFile.SetTX1D(Selected, NewName);
                }
                else if (chosentex == 3)
                {
                    std::cout << "\n TX2D " << MSHFile.Materials.at(Selected).TX2D
                        << " renamed to " << NewName << '\n';

                    // Now actually do it
                    MSHFile.SetTX2D(Selected, NewName);
                }
                else if (chosentex == 4)
                {
                    std::cout << "\n TX3D " << MSHFile.Materials.at(Selected).TX3D
                        << " renamed to " << NewName << '\n';

                    // Now actually do it
                    MSHFile.SetTX3D(Selected, NewName);
                }
            }
        }
        else if (ToEdit == 5)
        {
            std::cout << " Enter new Data0 Value:";
            short NewData0 = GetUserInput<short>();
            MSHFile.SetData0(Selected, NewData0);
        }
        else if (ToEdit == 6)
        {
            std::cout << " Enter new Data1 Value:";
            short NewData1 = GetUserInput<short>();
            MSHFile.SetData1(Selected, NewData1);
        }
        else if (ToEdit == 7)
        {
        bool done = false;
        while(!done)
            done = MaterialEditorAdvanced(MSHFile, Selected);
        }
        else if (ToEdit == 8)
            HelpView(4);
        else if (ToEdit == 9)
            return true;

        return false;
    }

    // Screen where the selected material can be viewed
    static inline void MaterialEditView(MSH &MSHFile, unsigned short Selected)
    {
        bool done = false;
        while (!done)
        {
            // Display properties of the selected material
            std::cout << "\n Selected Material: " << MSHFile.Materials.at(Selected).MatName << "\n Properties";

            // Display RenderType and rendertype name
            std::cout << "\n ---------- \n RenderType: " 
                << short(MSHFile.Materials.at(Selected).RenderType)
                << " -> " 
                << Material::RenderTypeName[MSHFile.Materials.at(Selected).RenderType];

            std::cout << "\n Flags: ";
            // Display the values of all the flags
            for (short i = 0; i < 8; i++)
                std::cout << "\n " << std::get<0>(MSHFile.Materials.at(Selected).MatFlags[i])
                    << " = " << std::boolalpha << std::get<1>(MSHFile.Materials.at(Selected).MatFlags[i]);

            std::cout << "\n\n Data0: " << short(MSHFile.Materials.at(Selected).Data0)
                << "     Data1: " << short(MSHFile.Materials.at(Selected).Data1);


            std::cout << "\n\n Textures: ";
            if (!MSHFile.Materials.at(Selected).TX0D.empty())
                std::cout << "\n TX0D: " << MSHFile.Materials.at(Selected).TX0D;
            else
                std::cout << "\n TX0D: " << "NO TEXTURE SET";
            if (!MSHFile.Materials.at(Selected).TX1D.empty())
                std::cout << "\n TX1D: " << MSHFile.Materials.at(Selected).TX1D;
            if (!MSHFile.Materials.at(Selected).TX2D.empty())
                std::cout << "\n TX2D: " << MSHFile.Materials.at(Selected).TX2D;
            if (!MSHFile.Materials.at(Selected).TX3D.empty())
                std::cout << "\n TX3D: " << MSHFile.Materials.at(Selected).TX3D;

            // Actual menu to edit the selected material 
            done = MaterialEditor(MSHFile, Selected);
        }
    }

    // Screen where Materials and their properties can be viewed
    static inline void MaterialView(MSH &MSHFile)
    {
        while (true)
        {
            short option = 0;
            MSHFile.ListMaterials();

            // Prompt user for their command
            std::cout << "\n What would you like to do? (ENTER NUMBER)\n 1. Edit a Material   2. Add a Material"
                << "   3. Remove a Material   4. Help   5. Go Back" << std::endl;

            std::cin.ignore();
            option = GetUserInput<short>();

            // Perform the specified action
            if (option == 1)
            {
                // Loop Sentinel
                bool IsGood = false;

                while (!IsGood)
                {
                    // Prompt and get MATI
                    std::cout << " Enter the material index of the material you wish to edit: ";
                    unsigned short Selected = GetUserInput<unsigned short>();

                    // Check that the MATI is valid
                    if (Selected < MSHFile.MaterialCount && Selected >= 0)
                    {
                        // Actual menu where the selected material can be edited
                        MaterialEditView(MSHFile, Selected);
                        IsGood = true;
                    }
                    else
                        std::cout << " Invalid Selection! Try again!\n";
                }
            }
            else if (option == 2)
                MaterialAdder(MSHFile);
            else if (option == 3)
            {
                // Loop Sentinel
                bool IsGood = false;
                unsigned short Selected = 0;

                while (!IsGood)
                {
                    // Prompt and get MATI
                    std::cout << " Enter the material index of the material you wish to remove: ";
                    Selected = GetUserInput<unsigned short>();

                    // Check that the MATI is valid
                    if (Selected < MSHFile.MaterialCount && Selected >= 0)
                    {
                        if (MSHFile.MaterialCount == 1)
                        {
                            std::cout << "\n Are you crazy?! You can't remove the only material!";
                            break;
                        }
                        else
                            IsGood = true;
                    }
                    else
                        std::cout << " Invalid Selection! Try again!\n";
                }
                if (MSHFile.MaterialCount > 1)
                {
                    // Prompt to be sure
                    std::cout << "\n Are you sure? Y/N";
                    unsigned char response = GetUserInput<unsigned char>();
                    response = std::tolower(response);
                    if (response == 'y')
                    {
                        std::cout << "\n Material " << MSHFile.Materials.at(Selected).MatName << " Removed!"
                            << "\n Note: Any models assigned to this material were reassigned to the first material\n ";

                        // Reassign any models that were assigned this material to mat 0
                        for (unsigned short F = 0; F < MSHFile.ModelCount; F++)
                            for (unsigned short G = 0; G < MSHFile.Models.at(F).Segments.size(); G++)
                                if (MSHFile.Models.at(F).Segments.at(G).MATI == Selected)
                                {
                                    MSHFile.Models.at(F).Segments.at(G).MATI = 0;
                                    MSHFile.Models.at(F).CHANGED[4] = true;
                                }

                        // Fix the material index of any materials after this one
                        for (unsigned short E = Selected; E < MSHFile.MaterialCount; E++)
                            MSHFile.Materials.at(E).MATI -= 1;

                        MSHFile.Materials.erase(MSHFile.Materials.begin() + Selected);
                        MSHFile.MaterialCount--;
                    }
                }
            }
            else if (option == 4)
                HelpView(4);
            else if (option == 5)
                return;
        }
    }

    // Screen where Models and their properties can be viewed and edited
    static inline void ModelView(MSH& MSHFile)
    {
        while (true)
        {
            short option = 0;
            MSHFile.ListModels();

            // Prompt user for their command
            std::cout << "\n What would you like to do? (ENTER NUMBER)\n 1. Edit a Model   2. Import a Model"
                << "   3. Export a Model   4. Remove a Model   5. Help   6. Go Back" << std::endl;

            std::cin.ignore();
            // Repeat getting user input until a valid choice is selected
            option = GetUserInput<short>();

            if (option == 1)
            {
                // Loop Sentinel
                bool IsGood = false;

                while (!IsGood)
                {
                    // Prompt and get MNDX
                    std::cout << " Enter the model index of the model you wish to edit: ";
                    unsigned short Selected = GetUserInput<unsigned short>();

                    // Check that the MNDX is valid
                    if (Selected <= MSHFile.ModelCount && Selected >= 1)
                    {
                        // Actual menu where the selected model can be edited
                        ModelEditView(MSHFile, Selected);
                        IsGood = true;
                    }
                    else
                        std::cout << " Invalid Selection! Try again!\n";
                }
            }
            else if (option == 2)
                if (!MSHFile.ImportMODL())
                    std::cout << "\n Error reading file!\n";
            else if (option == 3)
                std::cout << "\n Not ready yet!\n";
            else if (option == 4)
            {
                bool IsGood = false;

                while (!IsGood)
                {
                    // Prompt and get MNDX
                    std::cout << " Enter the model index of the model you wish to remove: ";
                    unsigned short Selected = GetUserInput<unsigned short>();

                    // Check that the MNDX is valid
                    if (Selected <= MSHFile.ModelCount && Selected >= 1)
                    {
                        if (MSHFile.ModelCount == 1)
                            std::cout << "\n Are you crazy?! You can't remove the only modl chunk!\n";
                        else
                        {
                            Selected--;
                            // Prompt to be sure
                            std::cout << "\n Are you sure? Y/N";
                            unsigned char response = GetUserInput<unsigned char>();
                            response = std::tolower(response);

                            if (response == 'y')
                            {
                                // remove modl and reassign indices for modls after the selected
                                for (unsigned short f = Selected; f < MSHFile.ModelCount; f++)
                                    MSHFile.Models.at(f).MNDX--;

                                std::cout << "\n Model " << MSHFile.Models.at(Selected).Name << " deleted!";

                                // Reassign any children MODLS to the first MODL
                                if (MSHFile.ModelCount > 2)
                                {
                                    for (unsigned short g = 0; g < MSHFile.ModelCount; g++)
                                        if (MSHFile.Models.at(g).PRNT_Index == Selected)
                                        {
                                            MSHFile.Models.at(g).PRNT_Index = 1;
                                            MSHFile.Models.at(g).PRNT = MSHFile.Models.at(1).Name;
                                            MSHFile.Models.at(g).PRNT_Size = MSHFile.Models.at(1).Name_Size;
                                        }
                                }
                                else
                                {
                                    // If there are only two MODLs, remove PRNT chunk from other
                                    if (Selected == 1)
                                    {
                                        MSHFile.Models.at(2).PRNT = "";
                                        MSHFile.Models.at(2).PRNT_Index = 1;
                                        MSHFile.Models.at(2).PRNT_Size = 0;
                                    }
                                    else
                                    {
                                        MSHFile.Models.at(1).PRNT = "";
                                        MSHFile.Models.at(1).PRNT_Index = 1;
                                        MSHFile.Models.at(1).PRNT_Size = 0;
                                    }
                                }

                                // Ok now actually do it
                                MSHFile.Models.erase(MSHFile.Models.begin() + Selected);
                                MSHFile.ModelCount--;
                                IsGood = true;
                            }
                            else
                                IsGood = true;
                        }
                    }
                    else
                        IsGood = true;
                }
            }
            else if (option == 5)
                HelpView(1);
            else if (option == 6)
                return;
        }
    }

    // Screen where the selected model can be viewed
    static inline void ModelEditView(MSH& MSHFile, unsigned short Selected)
    {
        // Since Model Index starts at 1 and not 0
        Selected--;

        bool done = false;
        while (!done)
        {
            std::string visible = "Invisible";
            if (MSHFile.Models.at(Selected).FLGS == 0)
                visible = "Visible";

            // Display properties of the selected model
            std::cout << "\n Selected Model: " << MSHFile.Models.at(Selected).Name << "\n Properties";

            // Show Parent
            std::cout << "\n ---------- \n Parent: ";
            if (!MSHFile.Models.at(Selected).PRNT.empty())
                std::cout << MSHFile.Models.at(Selected).PRNT;
            else
                std::cout << "None";

            std::cout << "\n Visibility: " << visible;

            if (MSHFile.Models.at(Selected).CLTH)
                std::cout << "\n Cloth Texture: " << MSHFile.Models.at(Selected).CTEX << '\n';
            else
            {
                std::cout << "\n Material Clusters:";
                for (unsigned short S = 0; S < MSHFile.Models.at(Selected).Segments.size(); S++)
                    std::cout << "\n " << S << " | Assigned Material: "
                    << MSHFile.Materials.at(MSHFile.Models.at(Selected).Segments.at(S).MATI).MatName;
            }
            // IF SWCI (COLL PRIM) then give option to convert to diff primitive type
            // TODO

            // Actual menu to edit the selected material 
            done = ModelEditor(MSHFile, Selected);
        }
    }

    // Screen where advanced model properties such as Vertex Colors can be edited
    static inline bool ModelEditorAdvanced(MSH& MSHFile, unsigned short Selected)
    {
        
        // Only if there's a CLRB in this MODL
        for (unsigned short clus = 0; clus < MSHFile.Models.at(Selected).Segments.size(); clus++)
            if (MSHFile.Models.at(Selected).Segments.at(clus).CLRB_Present)
                std::cout << "\n Single Vertex Color:\n Color found in cluster " << clus << " | "
                << (unsigned short)MSHFile.Models.at(Selected).Segments.at(clus).CLRB[0] << ' '
                << (unsigned short)MSHFile.Models.at(Selected).Segments.at(clus).CLRB[1] << ' '
                << (unsigned short)MSHFile.Models.at(Selected).Segments.at(clus).CLRB[2] << ' '
                << (unsigned short)MSHFile.Models.at(Selected).Segments.at(clus).CLRB[3] << '\n';

        // Only if there's a CLRL in this MODL
        for (unsigned short clus = 0; clus < MSHFile.Models.at(Selected).Segments.size(); clus++)
            if (MSHFile.Models.at(Selected).Segments.at(clus).CLRL_Present)
            {
                std::cout << "\n Multiple Vertex Colors: ";
                std::cout << "\n Colors found in cluster " << clus << ":\n";
                // For each RGBA item
                for (unsigned int clr = 0; clr < MSHFile.Models.at(Selected).Segments.at(clus).CLRL_Count; clr++)
                {
                    for (unsigned short g = 0; g < 4; g++)
                        std::cout << ' ' << (unsigned short)MSHFile.Models.at(Selected).Segments.at(clus).CLRL.at(clr).at(g) << ' ';
                    std::cout << "\n ";
                }
            }

        std::cout << "\n\n Select property to edit (ENTER NUMBER)"
            "\n 1. Add Color   2. Remove Color   3. Edit Color   4. Remove ALL colors from model   5. Help   6. Go Back";

        unsigned short option = GetUserInput<unsigned short>("\n");

        if (option == 1)
        {
            unsigned short cluster = GetUserInput<unsigned short>("\n Enter cluster index:");
            SetRGBA(MSHFile, Selected, 'B', cluster);
        }
        else if (option == 2)
        {
            unsigned short cluster = GetUserInput<unsigned short>("\n Enter cluster index:");
            unsigned char response = GetUserInput<unsigned char>("\n Are you sure? Y/N:");
            response = std::tolower(response);
            if (response == 'y')
            {
                MSHFile.Models.at(Selected).Segments.at(cluster).CLRB[0] = 0;
                MSHFile.Models.at(Selected).Segments.at(cluster).CLRB[1] = 0;
                MSHFile.Models.at(Selected).Segments.at(cluster).CLRB[2] = 0;
                MSHFile.Models.at(Selected).Segments.at(cluster).CLRB[3] = 0;
                MSHFile.Models.at(Selected).Segments.at(cluster).CLRB_Present = false;
                MSHFile.Models.at(Selected).Segments.at(cluster).CLRL_Count = 0;
                MSHFile.Models.at(Selected).Segments.at(cluster).CLRL.clear();
                MSHFile.Models.at(Selected).Segments.at(cluster).CLRL_Present = false;
                MSHFile.Models.at(Selected).CHANGED[5] = true;
                MSHFile.Models.at(Selected).CHANGED[6] = true;
                MSHFile.Models.at(Selected).MODLChanged = true;
                std::cout << "\n Vertex colors removed from cluster!";
            }
        }
        else if (option == 3)
        {
            unsigned short cluster = GetUserInput<unsigned short>("\n Enter cluster index:");
            SetRGBA(MSHFile, Selected, 'E', cluster);
        }
        else if (option == 4)
        {
            unsigned char response = GetUserInput<unsigned char>("\n Are you sure? Y/N:");
            response = std::tolower(response);
            if (response == 'y')
            {
                MSHFile.RemoveColors(Selected);
                std::cout << "\n All vertex colors removed from model!";
            }
        }
        else if (option == 5)
            HelpView(1);
        else if (option == 6)
            return true;

        return false;
    }

    // Screen where the selected model can be edited by the user
    static inline bool ModelEditor(MSH& MSHFile, unsigned short Selected)
    {
        // Prompt user for action and get input
        if (MSHFile.Models.at(Selected).CLTH)
            std::cout << "\n\n Select property to edit (ENTER NUMBER)"
            "\n 1. Name   2. Parent   3. Visibility   4. Cloth Texture Name   5. Clusters   6. Advanced   7. Help   8. Go Back";
        else
            std::cout << "\n\n Select property to edit (ENTER NUMBER)"
            "\n 1. Name   2. Parent   3. Visibility   4. Clusters   5. Advanced   6. Help   7. Go Back";

        unsigned short ToEdit = GetUserInput<unsigned short>("\n");

        // Take the specified action
        if (ToEdit == 1)
        {
            std::string NewName;
            bool Valid = false;
            std::cin.ignore();

            do
            {
                std::cout << "\n Enter new model name: ";
                std::getline(std::cin, NewName, '\n');

                // If input can be read and is valid, continue
                if (!std::cin.bad() && NewName.length() > 0)
                    Valid = true;
                else
                {
                    std::cin.clear();
                    std::cin.ignore();
                    std::cout << " Error with input, please try again!\n\n";
                }

            } while (!Valid);

            std::cout << ' ' << MSHFile.Models.at(Selected).Name
                << " renamed to " << NewName << '\n';

            MSHFile.RenameModel(Selected, NewName);
        }
        else if (ToEdit == 2)
        {
            short NewPRNT = GetUserInput<short>("\n Enter model index of new parent:") - 1;
            MSHFile.SetModelParent(Selected, NewPRNT);
        }
        else if (ToEdit == 3)
        {
            // Make Visible or Hidden
            unsigned short Value = GetUserInput<unsigned short>("\n Choose Value (ENTER NUMBER)\n 1. Visible   2. Invisible \n");
            MSHFile.SetModelVisibility(Selected, Value);
        }
        else if (ToEdit == 4)
        {
            if (MSHFile.Models.at(Selected).CLTH)
            {
                // Edit cloth tex here
                std::string NewName;
                bool Valid = false;
                std::cin.ignore();

                do
                {
                    std::cout << "\n Enter new cloth texture name: ";
                    std::getline(std::cin, NewName, '\n');

                    // If input can be read and is valid, continue
                    if (!std::cin.bad() && NewName.length() > 0)
                        Valid = true;
                    else
                    {
                        std::cin.clear();
                        std::cin.ignore();
                        std::cout << " Error with input, please try again!\n\n";
                    }

                } while (!Valid);

                MSHFile.SetClothTex(Selected, NewName);
            }
            else if (MSHFile.Models.at(Selected).MTYP != 0 && MSHFile.Models.at(Selected).MTYP != 3)
            {
                // Edit segment properties here
                // Loop Sentinel
                bool IsGood = false;

                while (!IsGood)
                {
                    // Prompt and get cluster index
                    std::cout << " Enter the cluster number you wish to edit:";
                    unsigned short indx = GetUserInput<unsigned short>();

                    // Check that the cluster index is valid
                    if (indx < MSHFile.Models.at(Selected).Segments.size() && indx >= 0)
                    {
                        // Display all materials and prompt for reassignment
                        for (unsigned short M = 0; M < MSHFile.MaterialCount; M++)
                            std::cout << "\n " << M << " | " << MSHFile.Materials.at(M).MatName;

                        unsigned short mati = GetUserInput<unsigned short>("\n\n Enter the index of the material you want to assign:");

                        MSHFile.Models.at(Selected).Segments.at(indx).MATI = mati;
                        MSHFile.Models.at(Selected).MODLChanged = true;
                        MSHFile.Models.at(Selected).CHANGED[4] = true;
                        IsGood = true;
                    }
                    else
                        std::cout << " Invalid Selection! Try again!\n";
                }
            }
        }
        else if (ToEdit == 5)
            if (MSHFile.Models.at(Selected).CLTH && MSHFile.Models.at(Selected).MTYP != 0 && MSHFile.Models.at(Selected).MTYP != 3)
            {
                // Edit segment properties here
                // Loop Sentinel
                bool IsGood = false;

                while (!IsGood)
                {
                    // Prompt and get cluster index
                    std::cout << " Enter the cluster number you wish to edit:";
                    unsigned short indx = GetUserInput<unsigned short>();

                    // Check that the cluster index is valid
                    if (indx < MSHFile.Models.at(Selected).Segments.size() && indx >= 0)
                    {
                        // Display all materials and prompt for reassignment
                        for (unsigned short M = 0; M < MSHFile.MaterialCount; M++)
                            std::cout << "\n " << M << " | " << MSHFile.Materials.at(M).MatName;

                        unsigned short mati = GetUserInput<unsigned short>("\n\n Enter the index of the material you want to assign:");
                        MSHFile.SetClusterMaterial(Selected, indx, mati);
                        IsGood = true;
                    }
                    else
                        std::cout << " Invalid Selection! Try again!\n";
                }
            }
            else
            {
                std::cout << "\n Not ready yet!";
                //bool done = false;
                //while (!done)
                //    done = ModelEditorAdvanced(MSHFile, Selected);
            }
        else if (ToEdit == 6)
            if (MSHFile.Models.at(Selected).CLTH)
            {
                std::cout << "\n Not ready yet!";
                //bool done = false;
                //while (!done)
                //    done = ModelEditorAdvanced(MSHFile, Selected);
            }
            else
                HelpView(1);
        else if (ToEdit == 7)
            if (MSHFile.Models.at(Selected).CLTH)
                HelpView(1);
            else
                return true;
        else if (ToEdit == 8)
            if (MSHFile.Models.at(Selected).CLTH)
                return true;

        return false;
    }

    // Screen where MSH file changes will be written to file
    static inline void SaveView(MSH &MSHFile)
    {
        // Loop to ensure valid and correct input for filename
        // Sentinel
        bool Valid_Name = false;
        do
        {
            // Prompt user for filename of msh
            std::cout << " Enter new filename: ";
            std::string MSHFileName;

            // If input can be read, continue
            if (std::cin >> MSHFileName)
            {
                // Check if filename has .msh extension, if not add it
                if (MSHFileName.at(MSHFileName.size() - 1) == 'h'
                    && MSHFileName.at(MSHFileName.size() - 2) == 's'
                    && MSHFileName.at(MSHFileName.size() - 3) == 'm'
                    && MSHFileName.at(MSHFileName.size() - 4) == '.')
                    // Assign the MSH object's Filename member to input
                    MSHFile.FileName = MSHFileName;
                else
                    MSHFile.FileName = MSHFileName + ".msh";

                // Open File
                std::ifstream TestFile(MSHFile.FileName.c_str(), std::ios::in);
                if (TestFile.is_open())
                {
                    std::cout << "\n Warning! File already exists. Overwrite? Y/N:";
                    unsigned char response = GetUserInput<unsigned char>();
                    response = std::tolower(response);
                    if (response == 'y')
                        Valid_Name = true;

                    TestFile.close();
                }
                else
                    Valid_Name = true;// carry on as usual
            }
            else
                std::cout << " Error with input, please try again!\n\n";
        } while (!Valid_Name);

        // Make any necessary calculations and adjustments
        MSHFile.PrepMSHForWrite();
        // Write our new MSH to file!
        bool MSHWritten = MSHFile.WriteMSH();
        if (MSHWritten)
            std::cout << "\n SUCCESS! MSH file successfully written...\n";
        else
            std::cout << "\n OOPS! Something went wrong!\n Make sure you have write access!\n";

        std::cout << " \n Press Enter to return to main screen... ";
        std::cin.ignore();
        std::cin.get();
        std::cout << "\n";
    }

    // Screen where instructions on use are given
    static inline void HelpView(unsigned short View = 1)
    {
        if (View == 1)
            std::cout << " Display General Help here\n";
        else if (View == 2)
            std::cout << " Display Material Help here\n";
        else if (View == 3)
            std::cout << " Display Material2 Help here\n";
        else if (View == 4)
        {
            short option = 0;
            while (option != 5)
            {
                std::cout << "\n Material Editing\n"
                    " Materials have a set of properties that you can edit to change how the model with the "
                    "\n applied material appears ingame. (Except perhaps the name property, as it's only"
                    "\n viewable in a hex editor or modelling program.)"
                    "\n\n If you'd like to know about rendertypes, enter 1"
                    "\n If you want to know more about material flags, enter 2"
                    "\n If you want to know about the texture slots, enter 3"
                    "\n If you'd like to learn about Data0/Data1, enter 4"
                    "\n To return to the editor, enter 5";

                option = GetUserInput<short>("\n");

                if (option == 1)
                {
                    std::cout << "\n Rendertypes are special properties that determine how a material will be rendered. "
                        "\n For example rendertype 27 is BumpMap, meaning a bump map will be used to adjust lighting"
                        "\n on the model to appear more 3-dimensional. Certain rendertypes will need to have either"
                        "\n an additional texture assigned, or a value in Data0 or Data1 to work properly."
                        "\n Press enter to continue...";
                    std::cin.ignore();
                    std::cin.get();
                    std::cout << "\n\n";
                }
                else if (option == 2)
                {
                    std::cout << "\n Material flags are properties that determine how a material will be rendered. "
                        "\n Specular: Makes material appear shiny according to alpha channel of texture(s)"
                        "\n Additive: Type of transparency where the material will slightly \"glow\" according to background"
                        "\n PerPixel: Makes the game light the material per pixel instead of per vertex"
                        "\n Hardedged: Makes material appear either fully transparent or visible according to alpha channel of texture"
                        "\n Double: Type of transparency where both sides of the polygon with the applied material are visible"
                        "\n Single: Type of transparency where only one side of the polygon with the applied material is visible"
                        "\n Glow: Makes the material glow with bloom according to the alpha channel and color of texture"
                        "\n Emissive: Makes the material \"glow\" without bloom and is unaffected by lighting"
                        "\n Press enter to continue...";
                        std::cin.ignore();
                        std::cin.get();
                        std::cout << "\n\n";
                }
                else if (option == 3)
                {
                    std::cout << "\n There are four different texture slots for each material. The first one is TX0D"
                        "\n which is the base diffuse texture used for the material. TX1D, TX2D, and TX3D are all"
                        "\n extra slots for rendertypes that require them. For example rendertype 27 requires a"
                        "\n BumpMap texture to be in TX1D. Or rendertype 6 requires a cubemap in TX3D."
                        "\n Press enter to continue...";
                    std::cin.ignore();
                    std::cin.get();
                    std::cout << "\n\n";
                }
                else if (option == 4)
                {
                    std::cout << "\n Data0 and Data1 are extra flags used to adjust certain rendertypes/material flags"
                        "\n For rendertype 25 Energy (Pulsating) Data 0 is used to scale how much brighter the"
                        "\n material should glow, while Data 1 is used to control the pulsing frequency. For"
                        "\n scrolling rendertypes Data0 controls speed of horizontal scrolling, and Data1 vertical."
                        "\n Press enter to continue...";
                    std::cin.ignore();
                    std::cin.get();
                    std::cout << "\n\n";
                }
                else if (option == 5)
                    return;
            }
        }
    }

    // Main window where the user sees the option to edit either materials or models
    static inline void ChooseFunction(MSH &MSHFile)
    {
        // The user should never get further back then this screen
        while (true)
        {
            std::string choice;

            // Prompt the user for their command
            std::cout << " What would you like to do? (ENTER NUMBER)\n 1. Edit Materials   2. Edit Models   3. Save MSH   4. Help \n";

            // Repeat getting user input until a valid choice is selected
            while (choice != "1" && choice != "2" && choice != "3" && choice != "4" && choice != "exit" && choice != "quit")
                choice = GetUserInput<std::string>("\n");

            // Fix case-sensitive input
            for (auto ch : choice)
                ch = std::tolower(ch);

            // Perform the specified command
            if (choice == "1")
                MaterialView(MSHFile);
            else if (choice == "2")
                ModelView(MSHFile);
            else if (choice == "3")
                SaveView(MSHFile);
            else if (choice == "4")
                HelpView();
            // Check if the poor soul is trying to exit the program
            else if (choice == "exit" || choice == "quit")
                return;
            else
                std::cout << " Invalid choice! Please enter a number of one of the listed commands.\n";
        }
    }
};