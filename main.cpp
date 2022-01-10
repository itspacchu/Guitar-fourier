#include<raylib.h>
#include<iostream>
#include<math.h>
#include "complexFourier.h"

const int screenWidth = 800;
const int screenHeight = 450;

// extra globals
Vector2 mousePos = {0,0};
vector<complex<double>> fourierCoeffs;
vector<Vector2> fourierPoints;

//functions
const auto sinusoid = [](float a,float f,float phi,float t) { return a * sin(2 * PI * f * t + phi); };

d_vec CmpMagnitude(vector<complex<double>> myFFTOUTPUT)
{
   vector<double> myFFTOUTPUT_abs;
   for (int i = 0; i < myFFTOUTPUT.size(); i++)
   {
      myFFTOUTPUT_abs.push_back(abs(myFFTOUTPUT[i]));
   }
   return myFFTOUTPUT_abs;
}






// transform basis vector between screenspace and worldspace
Vector2 tb(Vector2 in){
    return {in.x, screenHeight-in.y};
}

void DrawSinusoid(float x, float y, float amplitude, float frequency, float phase,float width,float step=1,float thickness=2,Color col=(Color){255,234,233,255})
{ 
    frequency/=screenWidth; //normalize frequency
    for (float i = x; i < x+width; i += step){
        Vector2 start = (Vector2){ i, y + sinusoid(amplitude, frequency, phase, i) };
        Vector2 end = (Vector2){i + step , y + sinusoid(amplitude, frequency, phase, i + step) };
        DrawLineEx(start , end, thickness ,col);
    }
}


float ReferenceLineFunction(float t){

    Vector2 init = (Vector2){0,screenHeight/2};
    Vector2 end = (Vector2){screenWidth,screenHeight/2};
    DrawLineV(init,mousePos,(Color){255,255,255,125});
    DrawLineV(mousePos,end,(Color){255,255,255,125});
    if(t < mousePos.x){
        return (t - init.x)/(init.x - mousePos.x) * (init.y - mousePos.y) + init.y;
    }else{
        return (t - mousePos.x)/(end.x - mousePos.x) * (end.y - mousePos.y) + mousePos.y;
    }
    return 0;
}

void DrawSinusoids(float x, float y, float gain, float phase,float width,float step=1,float thickness=2,Color col=(Color){255,234,233,255}){
    if(fourierCoeffs.size() < 1){
        return;
    }
    for(int x = 0;x < screenWidth;x+=10){
        float value = abs(fourierCoeffs[0]);
        for(int fc=0;fc < fourierCoeffs.size();fc++){
            value += sinusoid(fourierCoeffs[fc].real(),fourierCoeffs[fc].imag(),phase,x);
       } 
    }
}


void Update(double deltaTime){
    if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON) || IsMouseButtonDown(MOUSE_LEFT_BUTTON)){ 
        mousePos = tb(GetMousePosition());
        DrawCircleV(mousePos, 15, (Color){255,234,233,120});
        vector<complex<double>> sampledCoeffs;

        for(int i=0;i<screenWidth;i+=10){
            sampledCoeffs.push_back(complex<double>(ReferenceLineFunction(i),0));
            DrawCircle(i,ReferenceLineFunction(i),5,(Color){255*i/screenWidth,234,233,120});
        }
        ComplexFourier cfourier;
        fourierCoeffs = cfourier.fft(sampledCoeffs);
    }
    DrawSinusoids(0,0,0.1,0,screenWidth,10,2,(Color){255,234,233,255});
}

int main(){
    InitWindow(screenWidth, screenHeight, "fourier guitar");
    SetTargetFPS(60);
    RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
    while(!WindowShouldClose()){
        BeginTextureMode(target);{
            ClearBackground((Color){42, 42, 42, 255});
            Update(double(GetFrameTime()));
        }
        EndTextureMode();

    BeginDrawing();
        ClearBackground(RAYWHITE);  // Clear screen background
        DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)target.texture.height }, (Vector2){ 0, 0 }, WHITE);
    EndDrawing();
    }
    CloseWindow();
    return 0;
}