
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_video.h>    
#include <stdio.h>
#include <time.h>

static SDL_Window* window;
static SDL_Renderer* renderer;
char* romName[64];


typedef struct {
    uint16_t opcode;
    uint8_t NNN;//12 bit constant;
    uint8_t NN;//8 bit constant
    uint8_t N; //4 bit constant
    uint8_t X;//4 bit register
    uint8_t Y;//4 bit register
}instructions;

typedef struct {
    uint32_t windowWidth;
    uint32_t windowHeight;
    uint32_t bgColor;
    uint32_t fgColor;
    uint32_t scaleMultipler;
    uint32_t instPerSec;
        

    
}config;

config* c;

typedef enum {
    QUIT,
    RUNNING,
    PAUSED
}emulatorState  ;

typedef struct {
    emulatorState state;
    uint8_t ram[4096];
    //pixeller on off olacak bool yeter. boyle yapmak yerine ramin bir kismina pointer de yapabilirim
    //burda butun pixelleri bastan boyamak yerine son renklerini tutacagin bi array 
    // olusturup yanlis renkteyse boyayabilirsin ilerde
    bool display[64*32]; 
    uint8_t registers[16];
    uint16_t I;
    uint16_t pc;
    uint16_t stack[12];
    uint16_t* stackPtr;
    instructions ins;
    uint8_t delayTimer;
    uint8_t soundTimer;
    bool keypad[16];
    const char* romName;
    bool draw;
        
}emulator;


void updateTimers(emulator *chip8) {
    if (chip8->delayTimer > 0) {
        chip8->delayTimer--;
    }
    if (chip8->soundTimer> 0) {
        chip8->soundTimer--;
        //play audio
    }
    else {
        //pause audio
    }
}
// initialize fonksiyonu yap
int initEmulator(emulator *chip8, const char romName) {
    const uint32_t entryPoint = 0x200;
    const uint8_t font[] = {
       0xF0, 0x90, 0x90, 0x90, 0xF0,   // 0   
       0x20, 0x60, 0x20, 0x20, 0x70,   // 1  
       0xF0, 0x10, 0xF0, 0x80, 0xF0,   // 2 
       0xF0, 0x10, 0xF0, 0x10, 0xF0,   // 3
       0x90, 0x90, 0xF0, 0x10, 0x10,   // 4    
       0xF0, 0x80, 0xF0, 0x10, 0xF0,   // 5
       0xF0, 0x80, 0xF0, 0x90, 0xF0,   // 6
       0xF0, 0x10, 0x20, 0x40, 0x40,   // 7
       0xF0, 0x90, 0xF0, 0x90, 0xF0,   // 8
       0xF0, 0x90, 0xF0, 0x10, 0xF0,   // 9
       0xF0, 0x90, 0xF0, 0x90, 0x90,   // A
       0xE0, 0x90, 0xE0, 0x90, 0xE0,   // B
       0xF0, 0x80, 0x80, 0x80, 0xF0,   // C
       0xE0, 0x90, 0x90, 0x90, 0xE0,   // D
       0xF0, 0x80, 0xF0, 0x80, 0xF0,   // E
       0xF0, 0x80, 0xF0, 0x80, 0x80,   // F
    };
    memset(chip8, 0, sizeof(emulator));
    memcpy(&chip8->ram, font, sizeof(font));
    
    
	//load rom
    FILE* rom = fopen(romName, "rb");
    //rom yoksa
    if (!rom) {
        printf("rom bulunamadi");
        return 1;
    }
    //rom cok buyukse
    fseek(rom, 0, SEEK_END);
    size_t romSize = ftell(rom);
    size_t maxSize = chip8->ram - entryPoint;

    if (maxSize < romSize) {
        printf("rom cok buyuk");
        return 1;
    }
    //romu rama yukle
    if (fread(&chip8->ram[entryPoint], romSize, 1, rom)!=1) {
        printf("rom yuklenmedi");
        return 1;
    }
    fclose(rom);
    chip8->state = RUNNING;
    chip8->romName = romName;
    chip8->pc = entryPoint;
    chip8->stackPtr = chip8->stack[0];


    return 0;
}

void updateScreen(emulator *chip8) {

    SDL_Rect rect = {
        .x = 0,
        .y = 0,
        .h = c->scaleMultipler,
        .w = c->scaleMultipler,
    };
    //bg color 32 byte her 8 bytte r g b a degerlerini kaydira kaydira aliyom gereksiz basamaklari &0xFF le atiyom
    uint8_t colorfgR = (c->fgColor >> 24) & 0xFF;
    uint8_t colorfgG = (c->fgColor >> 16) & 0xFF;
    uint8_t colorfgB = (c->fgColor >> 8) & 0xFF;
    uint8_t colorfgA = (c->fgColor >> 0) & 0xFF;
    uint8_t colorbgR = (c->bgColor >> 24) & 0xFF;
    uint8_t colorbgG = (c->bgColor >> 16) & 0xFF;
    uint8_t colorbgB = (c->bgColor >> 8) & 0xFF;
    uint8_t colorbgA = (c->bgColor >> 0) & 0xFF;

    //loopla butun pikselleri teker teker bas(bunun daha iyi yolu varsa ilerde duzelt)
    for (uint32_t i = 0; i < sizeof(chip8->display) / sizeof(bool); i++) {
        //displaya gore cizilecek pikselin konumunu hesapla
        rect.x = (i % c->windowWidth) * c->scaleMultipler;
        rect.y = (i / c->windowWidth) * c->scaleMultipler;

        //displayda siradaki eleman true ysa ciz
        if (chip8->display[i] == true) {
            SDL_SetRenderDrawColor(renderer, colorfgR, colorfgG, colorfgB, colorfgA);
            SDL_RenderFillRect(renderer,&rect);
        }
        else {
            SDL_SetRenderDrawColor(renderer, colorbgR, colorbgG, colorbgB, colorbgA);
            SDL_RenderFillRect(renderer, &rect);
        }

    }
    SDL_RenderPresent(renderer);


}
void handleInput(emulator* chip8) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            chip8->state = QUIT;
        case SDL_EVENT_KEY_DOWN:
            switch (event.key.key) {
            case SDLK_SPACE:
                chip8->state = PAUSED;
                //buraya emulatorun load state ini falan yazicaz

                 // Map qwerty keys to CHIP8 keypad
            case SDLK_1: chip8->keypad[0x1] = true; break;
            case SDLK_2: chip8->keypad[0x2] = true; break;
            case SDLK_3: chip8->keypad[0x3] = true; break;
            case SDLK_4: chip8->keypad[0xC] = true; break;

            case SDLK_Q: chip8->keypad[0x4] = true; break;
            case SDLK_W: chip8->keypad[0x5] = true; break;
            case SDLK_E: chip8->keypad[0x6] = true; break;
            case SDLK_R: chip8->keypad[0xD] = true; break;

            case SDLK_A: chip8->keypad[0x7] = true; break;
            case SDLK_S: chip8->keypad[0x8] = true; break;
            case SDLK_D: chip8->keypad[0x9] = true; break;
            case SDLK_F: chip8->keypad[0xE] = true; break;

            case SDLK_Z: chip8->keypad[0xA] = true; break;
            case SDLK_X: chip8->keypad[0x0] = true; break;
            case SDLK_C: chip8->keypad[0xB] = true; break;
            case SDLK_V: chip8->keypad[0xF] = true; break;

            default: break;




            }
        case SDL_EVENT_KEY_UP:
            switch (event.key.key) {
                // Map qwerty keys to CHIP8 keypad
            case SDLK_1: chip8->keypad[0x1] = false; break;
            case SDLK_2: chip8->keypad[0x2] = false; break;
            case SDLK_3: chip8->keypad[0x3] = false; break;
            case SDLK_4: chip8->keypad[0xC] = false; break;

            case SDLK_Q: chip8->keypad[0x4] = false; break;
            case SDLK_W: chip8->keypad[0x5] = false; break;
            case SDLK_E: chip8->keypad[0x6] = false; break;
            case SDLK_R: chip8->keypad[0xD] = false; break;

            case SDLK_A: chip8->keypad[0x7] = false; break;
            case SDLK_S: chip8->keypad[0x8] = false; break;
            case SDLK_D: chip8->keypad[0x9] = false; break;
            case SDLK_F: chip8->keypad[0xE] = false; break;

            case SDLK_Z: chip8->keypad[0xA] = false; break;
            case SDLK_X: chip8->keypad[0x0] = false; break;
            case SDLK_C: chip8->keypad[0xB] = false; break;
            case SDLK_V: chip8->keypad[0xF] = false; break;

            default: break;
            }

        }
    }
}
void emulateInstruction(emulator* chip8,config c) {
    //onceki koddan VF flag aktarilmis mi
    bool VFflag;
    
    //kodda degisken varsa diye doldur
    chip8->ins.NNN = chip8->ins.opcode & 0xFFF;
    chip8->ins.NN= chip8->ins.opcode & 0xFF;
    chip8->ins.N= chip8->ins.opcode & 0xF;
    chip8->ins.X = (chip8->ins.opcode >> 8) & 0x0F;
    chip8->ins.Y = (chip8->ins.opcode >> 4) & 0x0F;
    
    //normalde butun komutlari if else if diye alt alta yazacaktim fakat 0NNN gibi degiskenli komutlarda
    //sikinti cikiyor. 12 bit saga kaydirip sadece bastaki seye gore switch case yapicam o yuzden
    switch (chip8->ins.opcode >> 12 & 0xF) {
    case 0x0:
        if (chip8->ins.opcode == 0xE0) {
            memset(&chip8->display[0], false, sizeof(chip8->display));
            chip8->draw = true;
        }
        else if (chip8->ins.opcode == 0xEE) {
            //return u yaz sonra
            chip8->pc = *--(chip8->stackPtr);

            
        }
        break;
    case 0x1:
        chip8->pc = chip8->ins.NNN;
        break;
    case 0x2:
        //2NNN calls subroutine at NNN
        //bunu tam anlayamadim buna bak bi daha
        *chip8->stackPtr = chip8->pc;
        chip8->pc = chip8->ins.NNN;
        break;

    case 0x3:
        if (chip8->registers[chip8->ins.X] == chip8->ins.NN) {
            chip8->pc += 2;
        }
        break;
    case 0x4:
        if (chip8->registers[chip8->ins.X] != chip8->ins.NN) {
            chip8->pc += 2;       
        }
        break;
    case 0x5:
        if (chip8->registers[chip8->ins.X] == chip8->registers[chip8->ins.Y]) {
            chip8->pc += 2;
        }
        break;
    case 0x6:
        chip8->registers[chip8->ins.X] = chip8->ins.NN;
        break;
    case 0x7:
        chip8->registers[chip8->ins.X] = chip8->registers[chip8->ins.X] + chip8->ins.NN;
        break;
    case 0x8:
        switch (chip8->ins.N) {
        case 0:
            chip8->registers[chip8->ins.X] = chip8->registers[chip8->ins.Y];
            break;
        case 1:
            //8xy1  Vx or Vy
            //set vf so 0 
            chip8->registers[chip8->ins.X] |= chip8->registers[chip8->ins.Y];
            chip8->registers[0xF] = 0;
            break;
        case 2:
            //8xy2 Vx and Vy
                        //set vf so 0 

            chip8->registers[chip8->ins.X] &= chip8->registers[chip8->ins.Y];
            chip8->registers[0xF] = 0;
            break;
        case 3:
            //8xy3 Vx xor Vy
            //set vf so 0 

            chip8->registers[chip8->ins.X] ^= chip8->registers[chip8->ins.Y];
            chip8->registers[0xF] = 0;
            break;
        case 4:
            //8xy4 Vx + Vy
            
        {
            //case in icinde direkt boyle degisken olusturunca hata verdi curly brace gerekiyomus sebebini bilmiyorum
            uint16_t result = (uint16_t)chip8->registers[chip8->ins.X] + chip8->registers[chip8->ins.Y];
            if (result != (result & 0xF)) {
                VFflag = true;
            }
            chip8->registers[chip8->ins.X] = result & 0xF;
            chip8->registers[0xF] = VFflag;
            break;
        }
            
        case 5:
            //8xy5 Vx - Vy

        {
            //case in icinde direkt boyle degisken olusturunca hata verdi curly brace gerekiyomus sebebini bilmiyorum
            uint8_t result = chip8->registers[chip8->ins.X] - chip8->registers[chip8->ins.Y];
            if (result < 0 ) {
                VFflag = true;
            }
            chip8->registers[chip8->ins.X] = result;
            chip8->registers[0xF] = VFflag;
            break;
        }
        case 6:
            //8xy6 store least significent bit of vx on vf, shift right by 1 
            VFflag = chip8->registers[chip8->ins.X] & 1;
            chip8->ins.X = chip8->ins.X >> 1;
            chip8->registers[0xF] = VFflag;
            break;
        
        case 7:
            //8xy7 Vx = Vy-Vx
            if (chip8->registers[chip8->ins.X] <= chip8->registers[chip8->ins.Y]) {
                VFflag = true;
            }

            chip8->registers[chip8->ins.X] = chip8->registers[chip8->ins.Y] - chip8->registers[chip8->ins.X];
            chip8->registers[0xF] = VFflag;
            break;
            
        case 0xE:
            //store most significant bit on vfflag then shift left by 1
            VFflag = chip8->registers[chip8->ins.X] >>7 &1;
            chip8->ins.X = chip8->ins.X >> 1;
            chip8->registers[0xF] = VFflag;
            break;
        }
    case 0x9:
        if (chip8->registers[chip8->ins.X] == chip8->registers[chip8->ins.Y]) {
            //skip to next instruction
            chip8->pc += 2;
        }
    case 0xA:
        chip8->I = chip8->ins.NNN;
        break;
    case 0xB:
        chip8->pc = chip8->registers[0] + chip8->ins.NNN;
        break;
    case 0xC:
        
        chip8->registers[chip8->ins.X] = (rand() % 256) & chip8->ins.NN;
        break;
    case 0xD:
        //DXYN: draw at X,Y a 8 width and N height sprite
    {
        uint8_t Xcoord= chip8->registers[chip8->ins.X] % c.windowWidth;
        uint8_t Ycoord= chip8->registers[chip8->ins.Y] % c.windowHeight;
        //original x coordinates
        const uint8_t Xori = Xcoord;
        chip8->registers[0xF] = 0;

        //her sprite in n rowu icin loopa al
        for (int i = 0; i < chip8->ins.N; i++) {
            //get the next sprite
            uint8_t sprite = chip8->ram[chip8->I + i];
            //reset x for the next drawing
            Xcoord = Xori;
        
            for (int j = 7; j >= 0; j--) {
                // if the pixel is already on set the carry flag
                //burayi tam anlamadim ama assa yukari tek tek bakmaya yariyo kopya cektim
                bool* pixel = chip8->display[Ycoord * c.windowWidth + Xcoord];
                //sprite in cizilecek kismi
                const bool spritePart = (sprite & (1 << j));
                if (spritePart && *pixel) {
                    chip8->registers[0xF] = 1;
                }
                *pixel ^= spritePart;
                //stop drawing if it's on edge
                if (++Xcoord >= c.windowWidth) {
                    break;
                }
                if (++Ycoord >= c.windowHeight) {
                    break;
                }
                chip8->draw = true;
                break;

                
            }
        
        }
    }
    case 0xE:
        switch (chip8->ins.N) {
        case 0xE:
            //skips to the next instruction if the key stored in VX is pressed
            if (chip8->keypad[chip8->registers[chip8->ins.X]]) {
                chip8->pc += 2;
            }
            break;
        case 1:
            if (!chip8->keypad[chip8->registers[chip8->ins.X]]) {
                chip8->pc += 2;
            }
            break;
        }
    case 0xF:
        switch (chip8->ins.N) {
        case 7:
            chip8->registers[chip8->ins.X] = chip8->delayTimer;
            break;
        case 0xA:
            //wait for keypress then store in VX
        {
            bool isPressed = false;
            uint8_t pressedKey = NULL;
            for (int i = 0; pressedKey == NULL && sizeof(chip8->keypad); i++) {
                if (chip8->keypad[i]) {
                    isPressed == true;
                    pressedKey = i;
                    break;
                }
            }
            //eger key basilmamissa veya basilan key birakilmamissa bu instructionu tekrar dondur
            if (!isPressed || chip8->keypad[pressedKey]) {
                chip8->pc -= 2;
            }
            else {
                chip8->registers[chip8->registers[chip8->ins.X]] = pressedKey;
                //reset key(burda null olursa sikinti olur mu bilmiyom)
                pressedKey = NULL;
                isPressed = false;
            }
            
            break;

        }
        case 0xE:
            chip8->I = chip8->registers[chip8->ins.X];
        case 5:
            switch (chip8->ins.NN) {
            case 15:
                //sets delay timer to VX
                chip8->delayTimer = chip8->registers[chip8->ins.X];
            case 55:
                //stores from v0 to vx in memory starting at address I.
                for(int i = 0; i < chip8->registers[chip8->ins.X]; i++){
                    // her cagirildiktan sonra I yi arttir
                    chip8->ram[chip8->I++] = chip8->registers[i];
                }
                break;
            case 65:
                //fills from v0 to vx from memory starting add address I.
                for (int i = 0; i < chip8->registers[chip8->ins.X]; i++) {
                    // her cagirildiktan sonra I yi arttir
                    chip8->registers[i] = chip8->ram[chip8->I++];
                }
                break;
            }
        case 8:
            //sets sound timer to VX
            chip8->soundTimer = chip8->registers[chip8->ins.X];
        case 3:
        }
    
    

    
    
    }
    
    

    


}


//bitti
void clearScreen() {
    const uint8_t r = (c->bgColor >> 24) & 0xFF;
    const uint8_t g = (c->bgColor >> 16) & 0xFF;
    const uint8_t b = (c->bgColor >> 8) & 0xFF;
    const uint8_t a = (c->bgColor >> 0) & 0xFF;
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
}


void quit() {
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
int main() {
    srand(time(NULL));
    //bunu free etmeyi unutma 
    c = malloc(sizeof(config));
    *c = (config){
        .windowHeight = 32,
        .windowWidth = 64,
        .instPerSec= 600,
        .scaleMultipler=20,
        .bgColor = 0x000000,
        .fgColor = 0xFFFFFF,
    };

    
    emulator *chip8;
    
    window = SDL_CreateWindow("cip8", c->windowWidth*c->scaleMultipler, c->windowHeight * c->scaleMultipler,0);
    renderer = SDL_CreateRenderer(window, NULL);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    while (chip8->state != QUIT) {
        handleInput(chip8);
        int64_t frameStartTime = SDL_GetPerformanceCounter();
        for (uint32_t i = 0; c->instPerSec / 60; i++) {
            emulateInstruction(chip8);

            //eger bisey cizilecekse bu frame de sadece cizim yap digerlerini atla
            //bu neden bilmiyorum
            if (chip8->ins.opcode >> 12 == 0xD) {
                break;
            }


        }
        int64_t frameEndTime = SDL_GetPerformanceCounter();
        //bu getperformancefrequency in olayi ne
        double timeElapsed = (double)((frameStartTime - frameEndTime) * 1000) / SDL_GetPerformanceFrequency();



        //put delay for 60hz. 1000/60 = 16.666666 ama 16 yazacam. substract the time needed for the code from 16.
        SDL_Delay(16.67f > timeElapsed? 16.67f - timeElapsed: 0);


        //update screen
        if (chip8->draw) {
            updateScreen(chip8);
            chip8->draw = false;
        }

        updateTimers;
    }
    quit();
    free(c);
    return 0;
}