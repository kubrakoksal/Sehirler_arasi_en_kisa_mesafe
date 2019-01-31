#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <windows.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
#include <allegro5\allegro_primitives.h>


int sehirsayisi=0;
char yolbirlestir[300];

typedef struct baglanti
{

    int kod,mesafe;
    struct baglanti * sonraki;


} baglanti;

void yolyaz(int);
void haritadagoster();
void yolbul(int);

int * mesafeler;
int * yollar;
int *visited;
char **sehirisimleri;


int s1=-1,s2=-1,cizilenyol=-1;

const float FPS = 60;

int main()
{

    FILE * skod =fopen("sehirkod.txt","r");
    if(skod==NULL)
    {
        printf("Dosya bulunamadi\n");
        exit(0);
    }

    FILE * smesafe =fopen("sehirmesafe.txt","r");
    if(smesafe==NULL)
    {
        printf("Dosya bulunamadi\n");
        exit(0);
    }

    fseek(smesafe,22,SEEK_SET);
    fseek(skod,11,SEEK_SET);
    char  *sehir1;
    char  *sehir2;
    char  *sehirkontrol;

    char parcala[40];
    char parcala2[20];
    while(fgets(parcala2,20,skod)!=NULL)
        sehirsayisi++;

    baglanti **sehirler=malloc(sizeof(baglanti*)*sehirsayisi);
    int i;

    sehirisimleri=malloc(sizeof(char*)*sehirsayisi);
    int sehirkodu;
    fseek(skod,11,SEEK_SET);
    char * temp;

    while(fgets(parcala2,20,skod)!=NULL)
    {
        sehirkontrol=strtok(parcala2,"-");
        temp=malloc(sizeof(char)*strlen(sehirkontrol)+1);
        strcpy(temp,sehirkontrol);
        sehirkodu=atoi(strtok(NULL,"-"));
        sehirisimleri[sehirkodu-1]=temp;
    }


    for(i=0; i<sehirsayisi; i++)
    {
        sehirler[i]=NULL;

    }

    fseek(skod,11,SEEK_SET);
    int sehir1kod=-1,sehir2kod=-1;
    while(fgets(parcala,40,smesafe)!=NULL)
    {
        sehir1=strtok(parcala,"-");
        sehir2=strtok(NULL,"-");
        int mesafe=atoi(strtok(NULL,"-"));

        for(i=0; i<sehirsayisi; i++)
        {
            if(strcmp(sehir1,sehirisimleri[i])==0)
                sehir1kod=i+1;
            if(strcmp(sehir2,sehirisimleri[i])==0)
                sehir2kod=i+1;
        }

        fseek(skod,11,SEEK_SET);



        baglanti * temp1;
        baglanti * temp2;

        temp1=malloc(sizeof(baglanti));
        temp2=malloc(sizeof(baglanti));

        temp1->mesafe=mesafe;
        temp1->sonraki=NULL;
        temp2->sonraki=NULL;
        temp2->mesafe=mesafe;


        if(sehirler[sehir1kod-1]==NULL)
        {
            temp1->kod=sehir2kod;
            sehirler[sehir1kod-1]=temp1;
        }
        else
        {
            temp1->kod=sehir2kod;
            baglanti * gecici=sehirler[sehir1kod-1];
            while(gecici->sonraki!=NULL)
                gecici=gecici->sonraki;

            gecici->sonraki=temp1;
        }

        if(sehirler[sehir2kod-1]==NULL)
        {
            temp2->kod=sehir1kod;
            sehirler[sehir2kod-1]=temp2;
        }
        else
        {
            baglanti * gecici=sehirler[sehir2kod-1];
            temp2->kod=sehir1kod;
            while(gecici->sonraki!=NULL)
                gecici=gecici->sonraki;

            gecici->sonraki=temp2;
        }

    }

///graph kontrol(hata yok)
    /**
            baglanti *say;
            for(i=0; i<sehirsayisi; i++)
            {
                printf("%d ",i+1);
                say=sehirler[i];
                if(say==NULL)
                    printf("baglanti yok\n");
                while(say!=NULL)
                {
                    printf("%d-%d ",say->kod,say->mesafe);
                    say=say->sonraki;
                }

                printf("\n");

            }*/

    int secenek=3;
    while(secenek!=1&&secenek!=2)
    {
        printf("\n\t*IKI SEHIR ARASINDAKI UZAKLIK VE GECILEN SEHIRLER ~ 1,\n\t*BIR SEHRIN DIGER TUM SEHIRLERE OLAN UZAKLIGI VE GECILEN SEHIRLER ~2  \n");
        printf("\n\tIki ciktidan (1 veya 2) birini seciniz: ");
        scanf("%d",&secenek);
        system("cls");
    }


    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),243);

    for(i=0; i<sehirsayisi; i++)
    {
        printf("   %d-%s\n",i+1,sehirisimleri[i]);
    }

    if(secenek==1)
    {
        while(1)
        {
            printf("\nArasindaki uzakligi bulmak isteginiz iki sehrin kodunu aralarinda bi bosluk birakarak giriniz:  ");
            scanf("%d %d",&s1,&s2);

            if(s1>=1&&s1<=sehirsayisi&&s2>=1&&s2<=sehirsayisi)
            {
                break;
            }
            system("cls");

            for(i=0; i<sehirsayisi; i++)
            {
                printf("   %d-%s\n",i+1,sehirisimleri[i]);
            }


        }
    }
    else if(secenek==2)
    {
        while(1)
        {
            printf("\nBir baslangic sehri seciniz ve kodunu giriniz: ");
            scanf("%d",&s1);
            if(s1>=1&&s1<=sehirsayisi)
            {
                break;
            }
            system("cls");
            for(i=0; i<sehirsayisi; i++)
            {
                printf("   %d-%s\n",i+1,sehirisimleri[i]);
            }


        }

    }
    mesafeler=malloc(sizeof(int)*sehirsayisi);
    visited=malloc(sizeof(int)*sehirsayisi);
    yollar=malloc(sizeof(int)*sehirsayisi);




    for(i=0; i<sehirsayisi; i++)
    {
        if(i==s1-1)
        {
            visited[i]=1;
            mesafeler[i]=0;
        }
        else
        {
            visited[i]=0;
            mesafeler[i]=INT_MAX;
        }
        yollar[i]=-1;
    }
    baglanti* bul;
    int min,kod,yol=0,anlikkonum=s1-1;


    for(i=0; i<sehirsayisi; i++)
    {

        min=INT_MAX;

        bul=sehirler[anlikkonum];

        while(bul!=NULL)
        {
            kod=(bul->kod)-1;

            if(mesafeler[kod]>(bul->mesafe)+yol&&visited[kod]==0)
            {
                mesafeler[kod]=(bul->mesafe)+yol;
                yollar[kod]=anlikkonum;
            }
            bul=bul->sonraki;
        }
        int a,minkod;

        for(a=0; a<sehirsayisi; a++)
        {
            if(mesafeler[a]<min&&visited[a]==0)
            {
                min=mesafeler[a];
                minkod=a;
            }
        }


        yol=min;
        visited[minkod]=1;
        anlikkonum=minkod;


    }

    system("cls");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),245);

    if(s2==-1)
    {
        printf("\n");
        for(i=0; i<sehirsayisi; i++)
        {


            printf("  %s sehri icin mesafe: %d\n",sehirisimleri[i],mesafeler[i]);
            printf("  Gecilen sehirler: ");

            yolyaz(i);
            printf("\n\n");


        }
        haritadagoster(secenek);
    }
    else
    {


        printf("\n  %s sehri icin mesafe:%d\n",sehirisimleri[s2-1],mesafeler[s2-1]);
        printf("  Gecilen sehirler:");
        yolyaz(s2-1);
        printf("\n");
        haritadagoster();


    }

    fclose(skod);
    fclose(smesafe);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),240);


    return 0;
}


void yolyaz(int kontrol)
{


    if(kontrol!=-1)
    {
        yolyaz(yollar[kontrol]);

        printf("%s ",sehirisimleri[kontrol]);
    }


}



void haritadagoster(int secenek)
{


    FILE * harita=fopen("haritaxy.txt","r");
    if(harita==NULL)
        printf("okunamadi\n");

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_BITMAP  *a   = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ///ALLEGRO_TIMER *timer = NULL;


    if(!al_init())
    {
        al_show_native_message_box(display, "Error", "Error", "Allegro tanimlanirken hata olustu.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        exit(0);
    }
    display = al_create_display(1525,785);
    if(!display)
    {
        al_show_native_message_box(display, "Error", "Error", "Ekran tanimlanirken hata olustu.",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        exit(0);
    }
    if(!al_install_mouse())
    {
        al_show_native_message_box(display, "Error", "Error", "Mouse tanimlanirken hata olustu.",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        exit(0);
    }

    event_queue = al_create_event_queue();
    if(!event_queue)
    {
        al_show_native_message_box(display, "Error", "Error", "HATA(failed to create event_queue!)",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        exit(0);
    }
    /*timer = al_create_timer(1.0 / FPS);
    if(!timer)
    {
        al_show_native_message_box(display, "Error", "Error", "HATA(failed to create timer!)",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        exit(0);
    }*/

    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon();// initialize the ttf (True Type Font) addon
    al_init_primitives_addon();


    al_install_keyboard();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    ///al_register_event_source(event_queue, al_get_timer_event_source(timer));
    ///al_start_timer(timer);

    al_register_event_source(event_queue, al_get_mouse_event_source());



    al_set_window_position(display,0,0);
    ALLEGRO_FONT* font =al_load_ttf_font("pirulen.ttf",30,0);
    if (!font)
    {
        fprintf(stderr, "'pirulen.ttf' yuklenemedi..\n");
        exit(0);
    }


    if(!al_init_image_addon())
    {
        al_show_native_message_box(display, "Error", "Error", "HATA(Failed to initialize al_init_image_addon!)",NULL, ALLEGRO_MESSAGEBOX_ERROR);
        exit(0);
    }
    al_set_window_title(display,"Türkiye Haritasi");



    a = al_load_bitmap("turkiyeharitasi.png");
    if(!a)
    {
        al_show_native_message_box(display, "Error", "Error", "Resim yuklenirken hata olustu.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
        exit(0);
    }
    al_draw_scaled_bitmap(a,0,0,al_get_bitmap_width(a), al_get_bitmap_height(a), 0, 0,1525, 785, 0);
    ALLEGRO_MOUSE_STATE state;

    int i,oncekiyol,x1,y1,x2,y2;
    char sehir1[50],sehir2[50];

    FILE * haritakoordinat=fopen("haritaxy.txt","r");
    if(haritakoordinat==NULL)
    {
        printf("dosya acilamadi\n");
        exit(0);
    }

    char string[30];
    int temp1,temp2;
    int var=0;

    for(i=0; i<sehirsayisi; i++)
    {
        if(s2!=-1)
        {
            i=s2-1;
            var=1;
        }
        oncekiyol=i;
        while(yollar[oncekiyol]!=-1)
        {
            strcpy(sehir1,sehirisimleri[oncekiyol]);
            strcpy(sehir2,sehirisimleri[yollar[oncekiyol]]);
            while(fscanf(haritakoordinat,"%s %d %d",string,&temp1,&temp2)==3)
            {
                if(strcmp(sehir1,string)==0)
                {
                    x1=temp1;
                    y1=temp2;

                }
                else if(strcmp(sehir2,string)==0)
                {
                    x2=temp1;
                    y2=temp2;
                }

            }


            al_draw_circle(x1,y1,2,al_map_rgb(0,0,0),5);
            al_draw_circle(x2,y2,2,al_map_rgb(0,0,0),5);

            al_draw_line(x1,y1,x2,y2,al_map_rgb(139,0,0),2);

            rewind(haritakoordinat);
            oncekiyol=yollar[oncekiyol];

            if(yollar[oncekiyol]==-1)
                al_draw_circle(x2,y2,2,al_map_rgb(255,255,0),5);


        }
        if(var==1)
        {
            break;

        }

    }
    font =al_load_ttf_font("pirulen.ttf",19,0);
    char mesafecevir1[50]="Aradaki mesafe: ";
    char mesafecevir2[6];

    if(s2==-1)
    {
        al_draw_rectangle(1400,720,950,750,al_map_rgb(0,0,0),2);
        al_draw_filled_rectangle(1400,720,950,750,al_map_rgb(216,191,216));

    }
    else
    {
        yolbul(s2-1);
        al_draw_text(font,al_map_rgb(128,0,0), 185,5,ALLEGRO_ALIGN_LEFT,yolbirlestir);
        al_draw_text(font,al_map_rgb(128,0,0), 190,30,ALLEGRO_ALIGN_LEFT,strcat(mesafecevir1,itoa(mesafeler[s2-1],mesafecevir2,10)));
    }



    while(1)
    {

        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
        else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN&&s2==-1)
        {

            al_get_mouse_state(&state);
            if((state.x<1400&&state.x>950)&&(state.y>700&&state.y<750))
            {
                if(cizilenyol!=-1)
                {
                    oncekiyol=cizilenyol;
                    while(yollar[oncekiyol]!=-1)
                    {
                        strcpy(sehir1,sehirisimleri[oncekiyol]);
                        strcpy(sehir2,sehirisimleri[yollar[oncekiyol]]);
                        while(fscanf(haritakoordinat,"%s %d %d",string,&temp1,&temp2)==3)
                        {
                            if(strcmp(sehir1,string)==0)
                            {
                                x1=temp1;
                                y1=temp2;

                            }
                            else if(strcmp(sehir2,string)==0)
                            {
                                x2=temp1;
                                y2=temp2;
                            }

                        }

                        al_draw_line(x1,y1,x2,y2,al_map_rgb(139,0,0),2);

                        rewind(haritakoordinat);
                        oncekiyol=yollar[oncekiyol];


                    }
                    al_flip_display();

                }


                al_draw_filled_rectangle(185,0,1520,25,al_map_rgb(255,255,255));
                al_draw_filled_rectangle(185,30,550,50,al_map_rgb(255,255,255));


                char  isim[50]="";
                i=0;
                ALLEGRO_EVENT_QUEUE* q = al_create_event_queue();
                al_register_event_source(q, al_get_keyboard_event_source());
                al_register_event_source(q, al_get_display_event_source(display));

                bool quit = false;
                int girme=0;
                while(!quit)
                {




                    al_draw_filled_rectangle(1400,720,950,750,al_map_rgb(216,191,216));
                    al_draw_text(font,al_map_rgb(128,0,0), 1175,725,ALLEGRO_ALIGN_CENTRE,isim);
                    al_flip_display();

                    ALLEGRO_EVENT e;
                    al_wait_for_event(q, &e);
                    if(e.type==ALLEGRO_EVENT_KEY_CHAR)
                    {
                        if(e.keyboard.unichar >= 32)
                        {
                            if(i!=49)
                            {
                                isim[i]=e.keyboard.unichar;
                                isim[i+1]='\0';
                                i++;
                            }

                        }
                        else if(e.keyboard.keycode == ALLEGRO_KEY_BACKSPACE)
                        {
                            if(i!=0)
                            {
                                isim[i-1]='\0';
                                i--;
                            }

                        }

                    }
                    else if(e.keyboard.keycode==ALLEGRO_KEY_ESCAPE)
                    {
                        quit=true;
                        girme=1;
                    }
                    else if(e.keyboard.keycode==ALLEGRO_KEY_ENTER)
                    {
                        quit=true;

                    }

                }

                int z;
                al_draw_filled_rectangle(1400,720,950,750,al_map_rgb(216,191,216));
                al_draw_filled_rectangle(1400,720,950,750,al_map_rgb(216,191,216));
                if(girme==0)
                {
                    char gecici1[50];
                    for(z=0; z<strlen(isim); z++)
                    {

                        if(z==0)
                            *(gecici1+0)=toupper(isim[0]);
                        else
                            *(gecici1+z)=tolower(isim[z]);
                    }
                    gecici1[strlen(isim)]='\0';
                    for(z=0; z<sehirsayisi; z++)
                    {



                        if(strcmp(gecici1,sehirisimleri[z])==0)
                        {
                            cizilenyol=z;

                            yolbul(z);
                            al_draw_text(font,al_map_rgb(128,0,0), 185,5,ALLEGRO_ALIGN_LEFT,yolbirlestir);
                            yolbirlestir[0]='\0';
                            al_draw_text(font,al_map_rgb(128,0,0), 190,30,ALLEGRO_ALIGN_LEFT,strcat(mesafecevir1,itoa(mesafeler[z],mesafecevir2,10)));
                            mesafecevir1[16]='\0';


                            oncekiyol=z;
                            while(yollar[oncekiyol]!=-1)
                            {
                                strcpy(sehir1,sehirisimleri[oncekiyol]);
                                strcpy(sehir2,sehirisimleri[yollar[oncekiyol]]);
                                while(fscanf(haritakoordinat,"%s %d %d",string,&temp1,&temp2)==3)
                                {
                                    if(strcmp(sehir1,string)==0)
                                    {
                                        x1=temp1;
                                        y1=temp2;

                                    }
                                    else if(strcmp(sehir2,string)==0)
                                    {
                                        x2=temp1;
                                        y2=temp2;
                                    }

                                }

                                al_draw_line(x1,y1,x2,y2,al_map_rgb(0,204,204),2);

                                rewind(haritakoordinat);
                                oncekiyol=yollar[oncekiyol];


                            }


                            al_flip_display();
                            break;

                        }





                    }

                }
            }


        }




        al_flip_display();
    }





    al_destroy_display(display);
    al_destroy_bitmap(a);
    al_destroy_event_queue(event_queue);


}

void yolbul(int kontrol)
{
    if(kontrol!=-1)
    {
        yolbul(yollar[kontrol]);
        strcat(yolbirlestir,sehirisimleri[kontrol]);
        strcat(yolbirlestir," ");
    }



}

