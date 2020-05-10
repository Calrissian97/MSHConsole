#include "MSH.h"
#include "View.h"

// Our main entry point
int main(int argc, char* argv[])
{
    // Attempt to load settings for DEBUG and ADVANCEDMODELS values
    std::ifstream ini("MSHConsole.ini");
    if (ini.is_open())
    {
        std::string line1;
        std::string line2;
        std::getline(ini, line1);
        std::getline(ini, line1);
        ini >> line1;
        ini >> line1;
        ini >> line1;
        std::getline(ini, line2);
        ini >> line2;
        ini >> line2;
        ini >> line2;

        ini.close();

        for (unsigned char ch : line1)
            ch = std::tolower(ch);

        for (unsigned char ch : line2)
            ch = std::tolower(ch);

        if (line1 == "true")
            DEBUG = true;
        else
            DEBUG = false;

        if (line2 == "true")
            ADVANCEDMODELS = true;
        else
            ADVANCEDMODELS = false;
    }

    // If commandline is used
    if (argc > 1)
    {
        if (argc > 2)
        {
            // Vector of MSH files to operate on
            std::vector<MSH> MSHARGS;

            unsigned short mati = 0;
            unsigned short mshi = 0;
            unsigned short clus = 0;
            unsigned short modl = 0;
            bool out = false;

            // For batch MSH file operations -----------------------------
            for (unsigned short arg = 1; arg < argc; arg++)
            {
                // Possible commands
                // Note: Flag commands toggle, everything else 'sets'
                // TODO: More error checking here
                if (argv[arg][0] != '-') // Interpret as MSH file
                {
                    MSH MSHFile;
                    MSHFile.SetMSHFilename(std::string(argv[arg]));
                    bool Read = MSHFile.ReadMSH();
                    if (Read)
                    {
                        MSHARGS.push_back(MSHFile);
                        mshi = static_cast<unsigned short>(MSHARGS.size()) - 1;
                    }
                }
                else if (std::string(argv[arg]) == "-help") // Output list of commands
                {
                    ;
                }
                else if (std::string(argv[arg]) == "-listmaterials")
                {
                    MSHARGS.at(mshi).ListMaterials();
                }
                else if (std::string(argv[arg]) == "-listmodels")
                {
                    MSHARGS.at(mshi).ListModels();
                }
                else if (std::string(argv[arg]) == "-msh") // If multiple MSHs, select MSH index to operate on
                {
                    mshi = std::stoi(std::string(argv[arg + 1]));
                    arg++;
                }
                else if (std::string(argv[arg]) == "-material") // If multiple materials, select material index to operate on
                {
                    mati = std::stoi(std::string(argv[arg + 1]));
                    arg++;
                }
                else if (std::string(argv[arg]) == "-model")
                {
                    modl = std::stoi(std::string(argv[arg + 1]));
                    arg++;
                }
                else if (std::string(argv[arg]) == "-cluster")
                {
                    clus = std::stoi(std::string(argv[arg + 1]));
                    arg++;
                }
                else if (std::string(argv[arg]) == "-out")
                {
                    std::string OutFile = argv[arg + 1];
                    MSHARGS.at(mshi).SetMSHFilename(OutFile);
                    arg++;
                    out = true;
                }
                else if (std::string(argv[arg]) == "-modelname")
                {
                    std::string NewName = argv[arg + 1];
                    MSHARGS.at(mshi).RenameModel(modl, NewName);
                    arg++;
                }
                else if (std::string(argv[arg]) == "-materialname")
                {
                    std::string NewName = argv[arg + 1];
                    MSHARGS.at(mshi).RenameMaterial(mati, NewName);
                    arg++;
                }
                else if (std::string(argv[arg]) == "-modelparent")
                {
                    unsigned short NewPrnt = std::stoi(argv[arg + 1]);
                    MSHARGS.at(mshi).SetModelParent(modl, NewPrnt);
                    arg++;
                }
                else if (std::string(argv[arg]) == "-modelvisibility")
                {
                    unsigned short vis = std::stoi(argv[arg + 1]);
                    MSHARGS.at(mshi).SetModelVisibility(modl, vis);
                    arg++;
                }
                else if (std::string(argv[arg]) == "-perpixel") // Toggle flag for material or mat 0 if not specified
                {
                    MSHARGS.at(mshi).SetFlag(mati, 3);
                }
                else if (std::string(argv[arg]) == "-specular") // Toggle flag for material or mat 0 if not specified
                {
                    MSHARGS.at(mshi).SetFlag(mati, 1);
                }
                else if (std::string(argv[arg]) == "-emissive") // Toggle flag for material or mat 0 if not specified
                {
                    MSHARGS.at(mshi).SetFlag(mati, 8);
                }
                else if (std::string(argv[arg]) == "-glow") // Toggle flag for material or mat 0 if not specified
                {
                    MSHARGS.at(mshi).SetFlag(mati, 7);
                }
                else if (std::string(argv[arg]) == "-single") // Toggle flag for material or mat 0 if not specified
                {
                    MSHARGS.at(mshi).SetFlag(mati, 6);
                }
                else if (std::string(argv[arg]) == "-double") // Toggle flag for material or mat 0 if not specified
                {
                    MSHARGS.at(mshi).SetFlag(mati, 5);
                }
                else if (std::string(argv[arg]) == "-hard") // Toggle flag for material or mat 0 if not specified
                {
                    MSHARGS.at(mshi).SetFlag(mati, 4);
                }
                else if (std::string(argv[arg]) == "-additive") // Toggle flag for material or mat 0 if not specified
                {
                    MSHARGS.at(mshi).SetFlag(mati, 2);
                }
                else if (std::string(argv[arg]) == "-clustermaterial")
                {
                    unsigned short mat = std::stoi(argv[arg + 1]);
                    MSHARGS.at(mshi).SetClusterMaterial(modl, clus, mat);
                    arg++;
                }
                else if (std::string(argv[arg]) == "-rt") // Set rendertype for material or mat 0 if not specified
                {
                    unsigned short NewRT = std::stoi(std::string(argv[arg + 1]));
                    MSHARGS.at(mshi).SetRT(mati, NewRT);
                    arg++;
                }
                else if (std::string(argv[arg]) == "-tx0d") // Set TX0D for material or mat 0 if not specified
                {
                    std::string NewTex = argv[arg + 1];
                    MSHARGS.at(mshi).SetTX0D(mati, NewTex);
                    arg++;
                }
                else if (std::string(argv[arg]) == "-tx1d") // Set TX1D for material or mat 0 if not specified
                {
                    std::string NewTex = argv[arg + 1];
                    MSHARGS.at(mshi).SetTX1D(mati, NewTex);
                    arg++;
                }
                else if (std::string(argv[arg]) == "-tx2d") // Set TX2D for material or mat 0 if not specified
                {
                    std::string NewTex = argv[arg + 1];
                    MSHARGS.at(mshi).SetTX2D(mati, NewTex);
                    arg++;
                }
                else if (std::string(argv[arg]) == "-tx3d") // Set TX3D for material or mat 0 if not specified
                {
                    std::string NewTex = argv[arg + 1];
                    MSHARGS.at(mshi).SetTX3D(mati, NewTex);
                    arg++;
                }
                else if (std::string(argv[arg]) == "-data0") // Set Data0 for material or mat 0 if not specified
                {
                    unsigned short NewVal = std::stoi(std::string(argv[arg + 1]));
                    MSHARGS.at(mshi).SetData0(mati, NewVal);
                    arg++;
                }
                else if (std::string(argv[arg]) == "-data1") // Set Data1 for material or mat 0 if not specified
                {
                    unsigned short NewVal = std::stoi(std::string(argv[arg + 1]));
                    MSHARGS.at(mshi).SetData1(mati, NewVal);
                    arg++;
                }
                else if (std::string(argv[arg]) == "-clothtexture")
                {
                    std::string NewClothTex = argv[arg + 1];
                    MSHARGS.at(mshi).SetClothTex(modl, NewClothTex);
                    arg++;
                }
                else if (std::string(argv[arg]) == "-speculardecay")
                {
                    unsigned int NewVal = std::stoi(std::string(argv[arg + 1]));
                    MSHARGS.at(mshi).SetSpecularDecay(mati, NewVal);
                    arg++;
                }
                else if (std::string(argv[arg]) == "-diffuse_bgra")
                {
                    float B = std::stof(std::string(argv[arg + 1]));
                    arg++;
                    float G = std::stof(std::string(argv[arg + 1]));
                    arg++;
                    float R = std::stof(std::string(argv[arg + 1]));
                    arg++;
                    float A = std::stof(std::string(argv[arg + 1]));
                    arg++;
                    float RGBA[4] = { R, G, B, A };
                    MSHARGS.at(mshi).SetDiffuseGBRA(mati, RGBA);

                }
                else if (std::string(argv[arg]) == "-ambient_bgra")
                {
                    float B = std::stof(std::string(argv[arg + 1]));
                    arg++;
                    float G = std::stof(std::string(argv[arg + 1]));
                    arg++;
                    float R = std::stof(std::string(argv[arg + 1]));
                    arg++;
                    float A = std::stof(std::string(argv[arg + 1]));
                    arg++;
                    float RGBA[4] = { R, G, B, A };

                    MSHARGS.at(mshi).SetAmbientGBRA(mati, RGBA);
                }
                else if (std::string(argv[arg]) == "-specular_bgra")
                {
                    float B = std::stof(std::string(argv[arg + 1]));
                    arg++;
                    float G = std::stof(std::string(argv[arg + 1]));
                    arg++;
                    float R = std::stof(std::string(argv[arg + 1]));
                    arg++;
                    float A = std::stof(std::string(argv[arg + 1]));
                    arg++;
                    float RGBA[4] = { R, G, B, A };

                    MSHARGS.at(mshi).SetSpecularGBRA(mati, RGBA);
                }
                //else if (std::string(argv[arg]) == "-vertexcolor_bgra")
                //{
                //    unsigned short B = std::stoi(std::string(argv[arg + 1]));
                //    arg++;
                //    unsigned short G = std::stoi(std::string(argv[arg + 1]));
                //    arg++;
                //    unsigned short R = std::stoi(std::string(argv[arg + 1]));
                //    arg++;
                //    unsigned short A = std::stoi(std::string(argv[arg + 1]));
                //    arg++;
                //    unsigned short RGBA[4] = { R, G, B, A };
                //
                //    MSHARGS.at(mshi).SetCLRB(modl, clus, RGBA);
                //}
                //else if (std::string(argv[arg]) == "-removecolors")
                //{
                //    MSHARGS.at(mshi).RemoveColors(modl);
                //}
            }

            // Export the MSHs
            for (unsigned short mshs = 0; mshs < MSHARGS.size(); mshs++)
            {
                if (MSHARGS.at(mshs).MSHChanged())
                {
                    // To prevent accidental overwriting
                    if (!out)
                    {
                        std::string NewName = MSHARGS.at(mshs).GetMSHFilename();
                        //if (std::regex_match(NewName, std::regex("(.*)(\\.msh)")))
                        NewName.at(NewName.size() - 4) = '_';
                        NewName.at(NewName.size() - 3) = 'n';
                        NewName.at(NewName.size() - 2) = 'e';
                        NewName.at(NewName.size() - 1) = 'w';
                        NewName.push_back('.');
                        NewName.push_back('m');
                        NewName.push_back('s');
                        NewName.push_back('h');

                        MSHARGS.at(mshs).SetMSHFilename(NewName);
                    }

                    MSHARGS.at(mshs).PrepMSHForWrite();
                    MSHARGS.at(mshs).WriteMSH();
                }
            }

            return 0;
        }

        // For single MSH operations -------------------------------------
        else
        {
            // Greetings and doc
            std::cout << " MSH Console by Calrissian97"
                "\n This program allows for simple edits to a msh file without"
                "\n requiring the user to use a hex editor or 3D modeling software."
                "\n --------------------------------------------------------------- \n\n";

            // For if the commandline was only used to open a file
            MSH MSHFile;
            MSHFile.SetMSHFilename(std::string(argv[1]));
            // Read the MSH file
            bool Read = MSHFile.ReadMSH();
            // Segments are not being read after awhile!!!
            if (Read)
                // Main screen where the user will interact
                View::ChooseFunction(MSHFile);
            else
            {
                std::cout << " \n\n File could not be opened!\n ";
                std::cin.get();
                return 1;
            }
        }
    }
    else
    {
        // Greetings and doc
        std::cout << " MSH Console by Calrissian97"
            "\n This program allows for simple edits to a msh file without"
            "\n requiring the user to use a hex editor or 3D modeling software."
            "\n --------------------------------------------------------------- \n\n";

        // If commandline was not used
        MSH MSHFile;

        // Set filename
        MSHFile.SetMSHFilename();

        // Read the MSH file
        bool Read = MSHFile.ReadMSH();
        if (Read)
            // Main screen where the user will interact
            View::ChooseFunction(MSHFile);
        else
        {
            std::cout << " \n\n File could not be opened!\n ";
            std::cin.get();
            return 1;
        }
    }

    // Exit without errors
    return 0;

}