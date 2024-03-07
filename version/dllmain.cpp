// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "Windows.h"
#include "version.h"
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include "Memory.h"
#include "GameData.h"
#include <conio.h> // For _getch() on Windows

HANDLE hMainThread;

// Function to initialize and show the console window
void InitializeConsole() {
    FILE* pFile = nullptr;
    AllocConsole();
    SetConsoleTitle(L"Helldivers 2 DLL Proxy PoC");
    freopen_s(&pFile, "CONOUT$", "w", stdout);
}

void DestroyConsole()
{
    DestroyWindow(GetConsoleWindow());
    FreeConsole();
}

//Check Key Pressed
bool keyPressed(int vKey)
{
    return (GetAsyncKeyState(vKey) & 1);
}

//Console Menu using checkbox
struct Checkbox {
    std::string title;
    bool checked;
};

// Function to display checkboxes
void displayCheckboxes(const std::vector<Checkbox>& checkboxes, size_t selectedCheckbox) {
    system("cls"); // Clear the console (Windows specific)

    printf("[Init] - Helldiver 2 PoC DLL Proxy...\n");
    printf("[Init] - Thanks to cfemen and gir489...\n");

    printf("[Ready] : Select some of the features below by pressing the [Space] key.\n");
    printf("[Ready] : Press [Enter] to run the feature you selected.\n");
    printf("[Ready] : After pressing [Enter], the selected features cannot be changed.\n");

    std::cout << "Checkboxes:\n";
    for (size_t i = 0; i < checkboxes.size(); ++i) {
        if (i == selectedCheckbox) {
            std::cout << "> ";
        }
        else {
            std::cout << "  ";
        }

        std::cout << "[" << (checkboxes[i].checked ? 'X' : ' ') << "] " << checkboxes[i].title << "\n";
    }
    std::cout << "\n";
}

DWORD WINAPI Payload(LPVOID lpParam)
{
    // Initialize the proxy for the DLL
    dllforward::setup();

    //Console Menu
    std::vector<Checkbox> checkboxes = { 
          {"Inf Health", false}
        , {"Inf Granades", false}
        , {"Inf Granades(Legit)", false}
        , {"Inf Ammo", false}
        , {"Inf Ammo(Legit)", false}
        , {"Inf Syringes", false}
        , {"Inf Syringes(Legit)", false}
        , {"Inf Stamina", false}
        , {"Inf Strategems", false}
        , {"Inf Mission Time", false}
        //, {"One / Two Hit Kill ( Bile Titan Bug, Aim Only Head )", false}
        , {"No Reload", false}
        , {"Max Resources", false}
        , {"Add 5 Samples", false}
        , {"No Recoil", false}
        , {"Inf Backpack", false}
        , {"Inf Special Weapon", false}
        , {"No Laser Cannon Overheat", false}
        , {"Instant Railgun", false}
        , {"Show All Map Icons", false}
        , {"No Stationary Turret Overheat", false}
        , {"No Backpack Shield Cooldown", false}
        , {"No JetPack Cooldown", false}
        , {"Unlock Armor/Weapons/Stratagems", false}
    
    }; // Initialize all checkboxes to unchecked
    const int numCheckboxes = checkboxes.size();
    size_t selectedCheckbox = 0;
    char userInput;

    HMODULE moduleHandle = nullptr;
    GameData gData;

    do
    {
        moduleHandle = GetModuleHandle(L"game.dll");
        Sleep(1000);
    } while (!moduleHandle);
    Sleep(100);


    //Show Console
    InitializeConsole();
    
    do {

        displayCheckboxes(checkboxes, selectedCheckbox);

        // Get user input
        userInput = _getch(); // Use _getch() for reading a single character without pressing Enter

        // Handle arrow key input
        switch (userInput) {
        case 72: // Up arrow key
            selectedCheckbox = (selectedCheckbox == 0) ? numCheckboxes - 1 : selectedCheckbox - 1;
            break;

        case 80: // Down arrow key
            selectedCheckbox = (selectedCheckbox == numCheckboxes - 1) ? 0 : selectedCheckbox + 1;
            break;

        case ' ': // Space bar
            // Toggle the state of the selected checkbox
            checkboxes[selectedCheckbox].checked = !checkboxes[selectedCheckbox].checked;
            break;

        case 13: // Enter key
            std::cout << "Activate Cheat Feature.\n";
            break;

        case 27: // Esc key

            break;

        default:
            // Ignore other key presses
            break;
        }

    } while (userInput != 13); // Loop until the Esc key is pressed

    for (size_t i = 0; i < checkboxes.size(); ++i) {
        if (checkboxes[i].checked)
        {

            if (checkboxes[i].title == "Inf Health")
            {
                if (!gData.InfHealth) // no need but its old code when activate using hotkey, but need to much hotkey for all feature
                {
                    BYTE InfHealthByte[] =
                    {
                        0x41, 0x51,
                        0x49, 0xB9, 0xE8, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x4D, 0x39, 0x48, 0x14, 
                        0x7C, 0x03,
                        0x45, 0x89, 0x38,
                        0x49, 0x8B, 0x84, 0xDE, 0x28, 0x04, 0x00, 0x00,
                        0x8B, 0x48, 0x10,
                        0x41, 0x59,
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,  // JMP [rip+6]
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Placeholder for the target address
                    };

                    BYTE InfHealthByte1[] =
                    {
                        0xB8, 0xE8, 0x03, 0x00, 0x00,
                        0x41, 0x39, 0x84, 0x8B, 0x40, 0x4C, 0x00, 0x00,
                        0x7F, 0x0D,
                        0xB8, 0x0F, 0x27, 0x00, 0x00,
                        0x41, 0x89, 0x84, 0x8B, 0x28, 0x4C, 0x00, 0x00,
                        0x48, 0x8B, 0x5C, 0x24, 0x20,
                        0x48, 0x8B, 0x74, 0x24, 0x28,
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,  // JMP [rip+6]
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Placeholder for the target address
                    };

                    uintptr_t InfHealth = Memory::FindPattern("game.dll", "45 89 38 49 8B 84 DE 28 04 00 00");
                    LPVOID memory = Memory::AllocateMemory(InfHealth, 0x100);
                    Memory::CreateTrampoline(InfHealth, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, InfHealth + 14, InfHealthByte, Memory::ArrayLength(InfHealthByte));

                    uintptr_t InfHealth1 = Memory::FindPattern("game.dll", "41 8B 84 8B 28 4C 00 00 48 8B 5C 24 20 48 8B 74 24 28");
                    memory = Memory::AllocateMemory(InfHealth1, 0x100);
                    Memory::CreateTrampoline(InfHealth1, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, InfHealth1 + 18, InfHealthByte1, Memory::ArrayLength(InfHealthByte1));

                    gData.InfHealth = !gData.InfHealth;
                    //create trampolin
                    printf("[Active] Infinite Health\n");
                }
            }

            if (checkboxes[i].title == "Inf Granades")
            {
                if (!gData.InfGrenades)
                {
                    uintptr_t GrenadesAddress = Memory::FindPattern("game.dll", "41 FF 08 4A 8B 84 ED");
                    Memory::Nop((LPVOID)(GrenadesAddress), 3);
                    gData.InfGrenades = !gData.InfGrenades;
                    printf("[Active] Infinite Grenades\n");
                }
            }

            if (checkboxes[i].title == "Inf Granades(Legit)")
            {
                if (!gData.InfGrenadesLegit && !gData.InfGrenades)
                {
                    uintptr_t GrenadesAddress = Memory::FindPattern("game.dll", "4D 03 C6 41 FF 08");
                    BYTE GrenadeBytes[] =
                    {
                        0x4D, 0x01, 0xF0,                               //add r8,r14
                        0x41, 0x83, 0x38, 0x01,                         //cmp dword ptr [r8],01
                        0x74, 0x03,                                     //je short @f
                        0x41, 0xFF, 0x08,                               // dec [r8]
                        0x4A, 0x8B, 0x84, 0xED, 0x30, 0x20, 0x00, 0x00, // mov rax,[rbp+r13*8+00002030]
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //JMP return_Grenades
                    };
                    LPVOID memory = Memory::AllocateMemory(GrenadesAddress, sizeof(GrenadeBytes));
                    Memory::CreateTrampoline(GrenadesAddress, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, GrenadesAddress + 14, GrenadeBytes, Memory::ArrayLength(GrenadeBytes));
                    gData.InfGrenadesLegit = !gData.InfGrenadesLegit;
                    printf("[Active] Infinite Grenades(Legit)\n");
                }
            }

            if (checkboxes[i].title == "Inf Ammo")
            {
                if (!gData.InfAmmo)
                {
                    uintptr_t AmmoAddress = Memory::FindPattern("game.dll", "41 83 2C C2 01 4D 8D 04 C2 49 8B 84 CA");
                    BYTE AmmoPatch[] = { 0x00 };
                    Memory::Patch((LPVOID)(AmmoAddress + 4), AmmoPatch, 1);
                    gData.InfAmmo = !gData.InfAmmo;
                    printf("[Active] Infinite Ammo\n");
                }
            }

            if (checkboxes[i].title == "Inf Ammo(Legit)")
            {
                if (!gData.InfAmmoLegit && !gData.InfAmmo)
                {
                    uintptr_t AmmoAddress = Memory::FindPattern("game.dll", "41 83 2C C2 01");
                    BYTE AmmoBytes[] =
                    {
                        0x41, 0x83, 0x3C, 0xC2, 0x02,                    // cmp dword ptr [r10+rax*8],#2
                        0x7E, 0x05,                                      // jle short @f
                        0x41, 0x83, 0x2C, 0xC2, 0x01,                    // sub dword ptr [r10+rax*8],01
                        0x4D, 0x8D, 0x04, 0xC2,			                 // lea r8,[r10+rax*8]
                        0x49, 0x8B, 0x84, 0xCA, 0x28, 0x20, 0x00, 0x00,  // mov rax,[r10+rcx*8+00002028]
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //JMP return_SubtractAmmo
                    };
                    LPVOID memory = Memory::AllocateMemory(AmmoAddress, sizeof(AmmoBytes));
                    Memory::CreateTrampoline(AmmoAddress, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, AmmoAddress + 17, AmmoBytes, Memory::ArrayLength(AmmoBytes));
                    gData.InfAmmoLegit = !gData.InfAmmoLegit;
                    printf("[Active] Infinite Ammo(Legit)\n");
                }
            }

            if (checkboxes[i].title == "Inf Syringes")
            {
                if (!gData.InfSyringes)
                {
                    uintptr_t Syringes = Memory::FindPattern("game.dll", "41 FF CF 3B C2 74 61");
                    Memory::Nop((LPVOID)(Syringes), 3);
                    gData.InfSyringes = !gData.InfSyringes;
                    printf("[Active] Infinite Syringes\n");
                }
            }

            if (checkboxes[i].title == "Inf Syringes(Legit)")
            {
                if (!gData.InfSyringesLegit && !gData.InfSyringes)
                {
                    uintptr_t SyringesAddress = Memory::FindPattern("game.dll", "48 03 C9 45 8B BC CA C8 75 00 00");
                    BYTE SyringesBytes[] =
                    {
                        0x48, 0x01, 0xC9,                                     // add rcx,rcx
                        0x45, 0x8B, 0xBC, 0xCA, 0xC8, 0x75, 0x00, 0x00,       // mov r15d,[r10+rcx*8+000075C8]
                        0x41, 0x83, 0xBC, 0xCA, 0xC8, 0x75, 0x00, 0x00, 0x01, // cmp dword ptr [r10+rcx*8+000075C8],01 { 1 }
                        0x74, 0x03,                                           // je short @f
                        0x41, 0xFF, 0xCF,                                     // dec r15d
                        0x39, 0xD0,                                           // cmp eax,edx
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //JMP return_Syringes
                    };
                    LPVOID memory = Memory::AllocateMemory(SyringesAddress, sizeof(SyringesBytes));
                    Memory::CreateTrampoline(SyringesAddress, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, SyringesAddress + 14, SyringesBytes, Memory::ArrayLength(SyringesBytes));
                    gData.InfSyringesLegit = !gData.InfSyringesLegit;
                    printf("[Active] Infinite Syringes(Legit)\n");
                }
            }

            if (checkboxes[i].title == "Inf Stamina")
            {
                if (!gData.InfStamina)
                {
                    uintptr_t Stamina = Memory::FindPattern("game.dll", "F3 41 0F 11 08 8B 48 10 E8 ?? ?? ?? ?? 41 8B 47 48");
                    BYTE StaminaPatch[] = { 0xF3, 0x41, 0x0F, 0x11, 0x30 };
                    Memory::Patch((LPVOID)(Stamina), StaminaPatch, 5);
                    gData.InfStamina = !gData.InfStamina;
                    printf("[Active] Infinite Stamina\n");
                }
            }

            if (checkboxes[i].title == "Inf Strategems")
            {
                if (!gData.InfStrategems)
                {
                    uintptr_t Strategems = Memory::FindPattern("game.dll", "0F 86 BF 01 00 00 0F");
                    BYTE StrategemsPatch1[] = { 0x90, 0xE9 };
                    Memory::Patch((LPVOID)(Strategems), StrategemsPatch1, 2);
                    gData.InfStrategems = !gData.InfStrategems;
                    printf("[Active] Infinite Strategems\n");
                }
            }

            if (checkboxes[i].title == "Inf Mission Time")
            {
                if (!gData.InfMissionTime)
                {
                    uintptr_t MissionTime = Memory::FindPattern("game.dll", "F3 0F 5C C7 F3 41 0F 5F C5");
                    Memory::Nop((LPVOID)(MissionTime), 4);
                    gData.InfMissionTime = !gData.InfMissionTime;
                    printf("[Active] Infinite Mission Time\n");
                }
            }

            if (checkboxes[i].title == "No Reload")
            {
                if (!gData.NoReload)
                {
                    BYTE NoReloadByte[] =
                    {
                        0x90,
                        0x49, 0x8B, 0x84, 0xCA, 0x28, 0x20, 0x00, 0x00,
                        0x8B, 0x48, 0x10,
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,  // JMP [rip+6]
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Placeholder for the target address
                    };

                    uintptr_t NoReload = Memory::FindPattern("game.dll", "41 89 28 49 8B 84 CA 28 20 00 00 8B 48 10");

                    LPVOID memory = Memory::AllocateMemory(NoReload, 0x100);
                    Memory::CreateTrampoline(NoReload, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, NoReload + 14, NoReloadByte, Memory::ArrayLength(NoReloadByte));
                    gData.NoReload = !gData.NoReload;
                    printf("[Active] No Reload\n");
                }
            }

            if (checkboxes[i].title == "Max Resources")
            {
                if (!gData.MaxResources)
                {
                    BYTE MaxResourcesByte[] =
                    {
                        0x41, 0x81, 0x84, 0x8A, 0xEC, 0x17, 0x00, 0x00, 0xF4, 0x01, 0x00, 0x00,
                        0x41, 0x81, 0x84, 0x8A, 0xF0, 0x17, 0x00, 0x00, 0xF4, 0x01, 0x00, 0x00,
                        0x41, 0x81, 0x84, 0x8A, 0xF4, 0x17, 0x00, 0x00, 0xF4, 0x01, 0x00, 0x00,
                        0x48, 0x8D, 0x82, 0x99, 0x00, 0x00, 0x00,
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,  // JMP [rip+6]
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // Placeholder for the target address
                    };

                    uintptr_t MaxResources = Memory::FindPattern("game.dll", "45 01 B4 8A EC 17 00 00");

                    LPVOID memory = Memory::AllocateMemory(MaxResources, 0x100);
                    Memory::CreateTrampoline(MaxResources, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, MaxResources + 17, MaxResourcesByte, Memory::ArrayLength(MaxResourcesByte));
                    gData.MaxResources = !gData.MaxResources;
                    printf("[Active] x500 Samples\n");
                }
            }

            if (checkboxes[i].title == "Add 5 Samples")
            {
                if (!gData.Add5Resources && !gData.MaxResources)
                {
                    uintptr_t ResourcesAddress = Memory::FindPattern("game.dll", "45 01 B4 8A EC 17 00 00");
                    BYTE ResourcesBytes[] =
                    {
                        0x41, 0xBE, 0x05, 0x00, 0x00, 0x00,              // mov r14d, #5
                        0x45, 0x01, 0xB4, 0x8A, 0xEC, 0x17, 0x00, 0x00,  // add [r10+rcx*4+000017EC],r14d
                        0x48, 0x8D, 0x82, 0x99, 0x00, 0x00, 0x00,        // lea rax,[rdx+00000099]
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 //JMP return_Add5Resources
                    };
                    LPVOID memory = Memory::AllocateMemory(ResourcesAddress, sizeof(ResourcesBytes));
                    Memory::CreateTrampoline(ResourcesAddress, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, ResourcesAddress + 15, ResourcesBytes, Memory::ArrayLength(ResourcesBytes));
                    gData.Add5Resources = !gData.Add5Resources;
                    printf("[Active] Add 5 Samples\n");
                }
            }

            if (checkboxes[i].title == "Instant Railgun")
            {
                if (!gData.InstantRailGun)
                {
                    BYTE InstantRailGunByte[] =
                    {
                        0x43, 0xC7, 0x84, 0x3E, 0x2C, 0x2C, 0x00, 0x00, 0xFC, 0xFF, 0x3F, 0x40,
                        0xF3, 0x43, 0x0F, 0x10, 0x84, 0x3E, 0x2C, 0x2C, 0x00, 0x00,
                        0x84, 0xC9, 0x74, 0x02, 0xEB, 0x0E,
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x71, 0x36, 0x4C, 0x80, 0x01, 0x00, 0x00, 0x00,
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    uintptr_t InstantRailGun = Memory::FindPattern("game.dll", "F3 43 0F 11 84 37 2C 2C 00 00");

                    LPVOID memory = Memory::AllocateMemory(InstantRailGun, 0x100);
                    Memory::CreateTrampoline(InstantRailGun, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, InstantRailGun + 14, InstantRailGunByte, Memory::ArrayLength(InstantRailGunByte));
                    gData.InstantRailGun = !gData.InstantRailGun;
                    printf("[Active] Instant Railgun\n");
                }
            }

            if (checkboxes[i].title == "No Recoil")
            {
                if (!gData.Recoil)
                {
                    BYTE RecoilByte[] =
                    {
                        0xEB
                    };

                    uintptr_t Recoil = Memory::FindPattern("game.dll", "44 8B 7C 24 ?? 41 3B 46 08 ");
                    Memory::Patch((LPVOID)(Recoil+9), RecoilByte, 1);
                    gData.Recoil = !gData.Recoil;
                    printf("[Active] No Recoil\n");
                }
            }

            if (checkboxes[i].title == "No Laser Cannon Overheat")
            {
                if (!gData.NoCannonOverheat)
                {
                    uintptr_t CannonOverheat = Memory::FindPattern("game.dll", "F3 0F 11 09 4C 8B C1 49");
                    Memory::Nop((LPVOID)(CannonOverheat), 4);
                    gData.NoCannonOverheat = !gData.NoCannonOverheat;
                    printf("[Active] No Cannon Laser Overheat\n");
                }
            }

            if (checkboxes[i].title == "Inf Special Weapon")
            {
                if (!gData.InfSpecWeapon)
                {
                    uintptr_t SpecWeapon = Memory::FindPattern("game.dll", "44 89 7F 08 41 80 BC 24");
                    Memory::Nop((LPVOID)(SpecWeapon), 4);
                    gData.InfSpecWeapon = !gData.InfSpecWeapon;
                    printf("[Active] Infinite Special Weapon\n");
                }
            }

            if (checkboxes[i].title == "No Stationary Turret Overheat")
            {
                if (!gData.NoStasTurretOverHeat)
                {
                    uintptr_t NoStasTurretOverHeat = Memory::FindPattern("game.dll", "F3 42 0F 11 84 32 ?? ?? ?? ?? 8B 55 38 43 89 94 35");
                    Memory::Nop((LPVOID)(NoStasTurretOverHeat), 10);
                    gData.NoStasTurretOverHeat = !gData.NoStasTurretOverHeat;
                    printf("[Active] No Stationary Turret Overheat\n");
                }
            }

            if (checkboxes[i].title == "No JetPack Cooldown")
            {
                if (!gData.JetpackNoCD)
                {
                    uintptr_t JetpackNoCD = Memory::FindPattern("game.dll", "8B 08 89 8C BE ?? ?? 00 00");
                    Memory::Nop((LPVOID)(JetpackNoCD + 2), 7);
                    gData.JetpackNoCD = !gData.JetpackNoCD;
                    printf("[Active] Jetpack No Cooldown\n");
                }
            }

            if (checkboxes[i].title == "No Backpack Shield Cooldown")
            {
                if (!gData.ShieldNoCD)
                {
                    BYTE ShieldNoCDByte[] =
                    {
                        0xF3, 0x0F, 0x5C, 0xC9, 0x90
                    };

                    uintptr_t ShieldNoCD = Memory::FindPattern("game.dll", "F3 41 0F 5C CA F3 0F 11 8C EE");
                    Memory::Patch((LPVOID)(ShieldNoCD), ShieldNoCDByte, 5);
                    gData.ShieldNoCD = !gData.ShieldNoCD;
                    printf("[Active] Backpack Shield No Cooldown\n");
                }
            }

            if (checkboxes[i].title == "Inf Backpack")
            {
                if (!gData.InfBackpack)
                {
                    uintptr_t Backpack = Memory::FindPattern("game.dll", "2B C6 4D 8D 85 48 04 00 00");
                    Memory::Nop((LPVOID)(Backpack), 2);
                    gData.InfBackpack = !gData.InfBackpack;
                    printf("[Active] Infinite Backpack\n");
                }
            }

            if (checkboxes[i].title == "Show All Map Icons")
            {
                if (!gData.ShowAllMapIcons)
                {
                    BYTE ShowAllMapIconsByte[] =
                    {
                        0xB8, 0x01, 0x00, 0x00, 0x00, 0x90
                    };

                    BYTE ShowAllMapIconsByte1[] =
                    {
                        0xF8
                    };

                    BYTE ShowAllMapIconsByte2n4[] =
                    {
                        0x90, 0xE9
                    };

                    BYTE ShowAllMapIconsByte3[] =
                    {
                        0xEB, 0x04
                    };

                    uintptr_t ShowAllMapIconsAddr = Memory::FindPattern("game.dll", "41 0F B6 44 97 23");
                    uintptr_t aob_CheckIfAlienHivesAreObstructed = Memory::FindPattern("game.dll", "41 80 BE 3C BA 07 00 00");
                    uintptr_t aob_CheckIfMinorInterestBlipIsDiscovered = Memory::FindPattern("game.dll", "0F 85 ?? ?? ?? ?? 48 8B 44 24 ?? 80 78 29 00");
                    uintptr_t aob_GetMinorInterestBlipIcon = Memory::FindPattern("game.dll", "0F 84 ?? ?? ?? ?? 48 8B 4C 24 ?? F3 41 0F 10 4F");
                    uintptr_t aob_CheckMissionBlip = Memory::FindPattern("game.dll", "0F 85 59 02 00 00 49 8D");
                     
                    Memory::Patch((LPVOID)(ShowAllMapIconsAddr), ShowAllMapIconsByte, 6);
                    Memory::Patch((LPVOID)(aob_CheckIfAlienHivesAreObstructed), ShowAllMapIconsByte1, 1);
                    Memory::Nop((LPVOID)(aob_CheckIfAlienHivesAreObstructed+1), 7);
                    Memory::Patch((LPVOID)(aob_CheckIfMinorInterestBlipIsDiscovered), ShowAllMapIconsByte2n4, 2);
                    Memory::Patch((LPVOID)(aob_GetMinorInterestBlipIcon), ShowAllMapIconsByte3, 2);
                    Memory::Patch((LPVOID)(aob_CheckMissionBlip), ShowAllMapIconsByte2n4, 2);
                    gData.ShowAllMapIcons = !gData.ShowAllMapIcons;
                    printf("[Active] Show All Map Icons\n");
                }
            }

            if (checkboxes[i].title == "Unlock Armor/Weapons/Stratagems")
            {
                if (!gData.UnlockStuff)
                {
                    const char* gameDll = "game.dll";
                    uintptr_t baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandleA(gameDll));
                    uintptr_t StrategemExtra = baseAddress + 0xB7B690;
                    BYTE StuffPatch[] = { 0xB0, 0x01, 0xC3 };
                    Memory::Patch(reinterpret_cast<LPVOID>(StrategemExtra), StuffPatch, 3);

                    uintptr_t Armory = baseAddress + 0xBB4AC0;
                    Memory::Patch(reinterpret_cast<LPVOID>(Armory), StuffPatch, 3);

                    uintptr_t AllArmors = baseAddress + 0xA91290;
                    Memory::Patch(reinterpret_cast<LPVOID>(AllArmors), StuffPatch, 3);

                    gData.UnlockStuff = !gData.UnlockStuff;
                    printf("[Active] Unlock Armor/Weapons/Stratagems\n");
                }
            }

            /*if (checkboxes[i].title == "One / Two Hit Kill ( Bile Titan Bug, Aim Only Head )")
            {
                if (!gData.OHK)
                {
                    BYTE OHKByte[] =
                    {
                        0x83, 0xBF, 0x38, 0x0B, 0x00, 0x00, 0x0A,
                        0x0F, 0x85, 0x05, 0x00, 0x00, 0x00,
                        0xE9, 0x18, 0x00, 0x00, 0x00,
                        0xC7, 0x87, 0x44, 0x64, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x66, 0x4A, 0x6B, 0x80, 0x01, 0x00, 0x00, 0x00,
                        0x89, 0x87, 0x44, 0x64, 0x00, 0x00,
                        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
                    };

                    uintptr_t OHK = Memory::FindPattern("game.dll", "89 87 44 64 00 00");
                    LPVOID memory = Memory::AllocateMemory(OHK, 0x100);
                    Memory::CreateTrampoline(OHK, memory);
                    Memory::WriteAssemblyInstructions((uintptr_t)memory, OHK + 15, OHKByte, Memory::ArrayLength(OHKByte));
                    gData.OHK = !gData.OHK;
                    printf("[Active] Instant Railgun\n");
                }
            }*/
            


        }
    }
    printf("[Exit] Unload\n");
    FreeConsole();
    FreeLibraryAndExitThread(GetModuleHandle(NULL), 0);
    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        hMainThread = CreateThread(NULL, 0, Payload, hModule, 0, NULL);
        if (hMainThread)
            CloseHandle(hMainThread);
    }break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        FreeConsole();
        FreeLibraryAndExitThread(hModule, 0);
        break;
    }
    return TRUE;
}

