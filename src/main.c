#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "SDL/SDL.h"
#include "SDL/SDL_net.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"

#define IDENT 0
#define INDEX 1
#define POS 2
#define ROUTE 3
#define PERF 4
#define MAINT 5
#define THRUST_LIM 6

SDL_Surface *screen, *cduImage, *cduImagePressed;

TTF_Font *fontBig, *fontSmall;

int pageNumber = IDENT;

TCPsocket sd, csd; /* Socket descriptor, Client socket descriptor */
IPaddress ip, *remoteIP;
int quit, quit2;
char buffer[512];
int speed, alt, pitch, roll, fuel;
char test[16] = { ' ', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0','\0','\0','\0' };
char inputword[4] = { ' ' };
char inputICAO[4];
const char clearString[4] = { ' ', '\0'};
char newChar;
////////////////INIT//////////////////
void init(char *title)
{
	/* Initialise video */
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Could not init sdl: %s\n", SDL_GetError());
		
		exit(1);
	}
	
	/* Open a 640 x 480 screen */
	screen = SDL_SetVideoMode(326, 512, 0, SDL_HWPALETTE|SDL_DOUBLEBUF);
	
	if(screen == NULL)
	{
		printf("Couldn't set screen mode to 640 x 480: %s\n", SDL_GetError());
		
		exit(1);
	}
	
	/* Initialise SDL_TTF */
	if (TTF_Init() < 0)
	{
		printf("Couldn't initialise SDL_TTF: %s\n", SDL_GetError());
		
		exit(1);
	}
	
	if(SDLNet_Init() < 0)
	{
		fprintf(stderr, "SDLNet_Init: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
  
	/* Resolving the host using NULL make network interface to listen */
	if (SDLNet_ResolveHost(&ip, NULL, 5555) < 0)
	{
		fprintf(stderr, "SDLNet_ResolveHost: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
  
	/* Open a connection with the IP provided (listen on the host's port) */
	if (!(sd = SDLNet_TCP_Open(&ip)))
	{
		fprintf(stderr, "SDLNet_TCP_Open: %s\n", SDLNet_GetError());
		exit(EXIT_FAILURE);
	}
	
	/* Set the screen title */
	SDL_WM_SetCaption(title, NULL);
}
void cleanup(void)
{
	/* free the image */
	if(cduImage != NULL)
	{
		SDL_FreeSurface(cduImage);
	}
	
	if(cduImagePressed != NULL)
	{
		SDL_FreeSurface(cduImagePressed);
	}	
	
	if(fontBig != NULL)
	{
		TTF_CloseFont(fontBig);
	}
	
	if(fontSmall != NULL)
	{
		TTF_CloseFont(fontSmall);
	}
	
	/* shutdown SDL */
	SDLNet_TCP_Close(csd);
	SDLNet_TCP_Close(sd);
	SDLNet_Quit();
	SDL_Quit();
}
typedef struct keys
{
	/*int number;*/
	int x, y, w, h;
	int active, charactercount;
} Keys;
///////////////KEY LIST///////////////
Keys a, b, c, d, e;
Keys f, g, h, i, j;
Keys k, l, m, n, o;
Keys p, q, r, s, t;
Keys u, v, w, x, y;
Keys z;
Keys l1, l2, l3, l4, l5, l6;
Keys r1, r2, r3, r4, r5, r6;

///////////////KEY ATRIB///////////////
void initKeys()
{
	/*a.number = 1;
	a.x = 136;
	a.y = 317;
	a.w = 22;
	a.h = 22;*/
	a.active = 0;
	b.active = 0;
	c.active = 0;
	d.active = 0;
	e.active = 0;
	f.active = 0;
	g.active = 0;
	h.active = 0;
	i.active = 0;
	j.active = 0;
	k.active = 0;
	l.active = 0;
	m.active = 0;
	n.active = 0;
	o.active = 0;
	p.active = 0;
	q.active = 0;
	r.active = 0;
	s.active = 0;
	t.active = 0;
	u.active = 0;
	v.active = 0;
	w.active = 0;
	x.active = 0;
	y.active = 0;
	z.active = 0;
	/* LEFT */
	l1.charactercount = 0;
	l1.x = 10;
	l1.y = 62;
	l1.w = 22;
	l1.h = 15;
	l2.charactercount = 0;
	l2.x = 10;
	l2.y = 88;
	l2.w = 22;
	l2.h = 15;
	l3.charactercount = 0;
	l3.x = 10;
	l3.y = 114;
	l3.w = 22;
	l3.h = 15;
	l4.charactercount = 0;
	l4.x = 10;
	l4.y = 140;
	l4.w = 22;
	l4.h = 15;
	l5.charactercount = 0;
	l5.x = 10;
	l5.y = 166;
	l5.w = 22;
	l5.h = 15;
	l6.charactercount = 0;
	l6.x = 10;
	l6.y = 196;
	l6.w = 22;
	l6.h = 15;
	
	/* RIGHT */
	r1.charactercount = 0;
	r1.x = 293;
	r1.y = 62;
	r1.w = 22;
	r1.h = 15;
	r2.charactercount = 0;
	r2.x = 293;
	r2.y = 88;
	r2.w = 22;
	r2.h = 15;
	r3.charactercount = 0;
	r3.x = 293;
	r3.y = 114;
	r3.w = 22;
	r3.h = 15;
	r4.charactercount = 0;
	r4.x = 293;
	r4.y = 140;
	r4.w = 22;
	r4.h = 15;
	r5.charactercount = 0;
	r5.x = 293;
	r5.y = 166;
	r5.w = 22;
	r5.h = 15;
	r6.charactercount = 0;
	r6.x = 293;
	r6.y = 196;
	r6.w = 22;
	r6.h = 15;
	r6.h = 15;
}
/////////////////FONT///////////////////
TTF_Font *loadFont(char *name, int size)
{
	/* Use SDL_TTF to load the font at the specified size */
	
	TTF_Font *font = TTF_OpenFont(name, size);
	
	if(font == NULL)
	{
		printf("Failed to open font %s: %s\n", name, TTF_GetError());
		
		exit(1);
	}
	return font;
}
void drawString(char *text, TTF_Font *font, SDL_Surface *destimage, int destX, int destY, int destW, int destH)
{
	SDL_Rect dest;
	SDL_Surface *surface;
	SDL_Color foregroundColor, backgroundColor;
	
	/* White text on a black background */
	
	foregroundColor.r = 39;
	foregroundColor.g = 194;
	foregroundColor.b = 39;
	
	backgroundColor.r = 0;
	backgroundColor.g = 0;
	backgroundColor.b = 0;

	/* Use SDL_TTF to generate a string image, this returns an SDL_Surface */

	surface = TTF_RenderUTF8_Shaded(font, text, foregroundColor, backgroundColor);
	
	if (surface == NULL)
	{
		//printf("Couldn't create String %s: %s\n", text, SDL_GetError());

		surface = TTF_RenderUTF8_Shaded(font, " ", foregroundColor, backgroundColor);

		return;
	}
	
	/* Blit the entire surface to the screen */

	dest.x = destX;
	dest.y = destY;
	dest.w = destW;
	dest.h = destH;

	SDL_BlitSurface(surface, NULL, destimage, &dest);
	
	/* Free the generated string image */

	SDL_FreeSurface(surface);
}
void charCopy(char *newChar)
{
	char tempString[4] = {'\0'};
	// int i = 0;
	if (l1.charactercount == 0)
	{
		strcpy(inputword, tempString);
		strcat(inputword, newChar);
	} else {
		strcat(inputword, newChar);
	}
	if (l1.charactercount == 4) 
	{
		l1.charactercount = 0;
		strcpy(inputword, clearString);
		strcpy(newChar, clearString);
		
	} else {
		l1.charactercount++;
	}
	//printf("%s:|:%s:|:%s:|:%d\n", newChar, inputword, inputICAO, l1.charactercount);
}
////////////////INPUT//////////////////
void getInput(void)
{
	SDL_Event event;
	
	int mouseX = 0, mouseY = 0;
	
	/* Loop throught waiting messages and procces them */
	while (SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			/* closing the window or pressing escape will exit */
			case SDL_QUIT:
				exit(0);
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						exit(0);
						break;
					case SDLK_a:
						a.active = 1;
						break;
					case SDLK_b:
						b.active = 1;
						break;
					case SDLK_c:
						c.active = 1;
						break;
					case SDLK_d:
						d.active = 1;
						break;
					case SDLK_e:
						e.active = 1;
						break;
					case SDLK_f:
						f.active = 1;
						break;
					case SDLK_g:
						g.active = 1;
						break;
					case SDLK_h:
						h.active = 1;
						break;
					case SDLK_i:
						i.active = 1;
						break;
					case SDLK_j:
						j.active = 1;
						break;
					case SDLK_k:
						k.active = 1;
						break;
					case SDLK_l:
						l.active = 1;
						break;
					case SDLK_m:
						m.active = 1;
						break;
					case SDLK_n:
						n.active = 1;
						break;
					case SDLK_o:
						o.active = 1;
						break;
					case SDLK_p:
						p.active = 1;
						break;
					case SDLK_q:
						q.active = 1;
						break;
					case SDLK_r:
						r.active = 1;
						break;
					case SDLK_s:
						s.active = 1;
						break;
					case SDLK_t:
						t.active = 1;
						break;
					case SDLK_u:
						u.active = 1;
						break;
					case SDLK_v:
						v.active = 1;
						break;
					case SDLK_w:
						w.active = 1;
						break;
					case SDLK_x:
						x.active = 1;
						break;
					case SDLK_y:
						y.active = 1;
						break;
					case SDLK_z:
						z.active = 1;
						break;
					default:
						break;
				}
			break;
			
			case SDL_KEYUP:
				switch(event.key.keysym.sym)
				{
					case SDLK_a:
						a.active = 0;
						break;
					case SDLK_b:
						b.active = 0;
						break;
					case SDLK_c:
						c.active = 0;
						break;
					case SDLK_d:
						d.active = 0;
						break;
					case SDLK_e:
						e.active = 0;
						break;
					case SDLK_f:
						f.active = 0;
						break;
					case SDLK_g:
						g.active = 0;
						break;
					case SDLK_h:
						h.active = 0;
						break;
					case SDLK_i:
						i.active = 0;
						break;
					case SDLK_j:
						j.active = 0;
						break;
					case SDLK_k:
						k.active = 0;
						break;
					case SDLK_l:
						l.active = 0;
						break;
					case SDLK_m:
						m.active = 0;
						break;
					case SDLK_n:
						n.active = 0;
						break;
					case SDLK_o:
						o.active = 0;
						break;
					case SDLK_p:
						p.active = 0;
						break;
					case SDLK_q:
						q.active = 0;
						break;
					case SDLK_r:
						r.active = 0;
						break;
					case SDLK_s:
						s.active = 0;
						break;
					case SDLK_t:
						t.active = 0;
						break;
					case SDLK_u:
						u.active = 0;
						break;
					case SDLK_v:
						v.active = 0;
						break;
					case SDLK_w:
						w.active = 0;
						break;
					case SDLK_x:
						x.active = 0;
						break;
					case SDLK_y:
						y.active = 0;
						break;
					case SDLK_z:
						z.active = 0;
						break;
					default:
						break;
				}
			break;
		
			case SDL_MOUSEBUTTONUP:
			
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				// get the mouse coords
				mouseX = event.button.x;
				mouseY = event.button.y;
				
				// collision for mouse and boxes
				// LEFT SIDE KEYS
				if(( mouseX > l1.x ) && ( mouseX < l1.x + l1.w ) && ( mouseY > l1.y ) && ( mouseY < l1.y + l1.h ))
				{
					if(pageNumber == INDEX)
					{
						pageNumber = IDENT;
						break;
					}
					if(pageNumber == POS)
					{
						strcpy(inputICAO, inputword);
						strcpy(inputword, clearString);	
						l1.charactercount = 0;
					}
					l1.charactercount = 0;
				}
				if(( mouseX > l2.x ) && ( mouseX < l2.x + l2.w ) && ( mouseY > l2.y ) && ( mouseY < l2.y + l2.h ))
				{
					if(pageNumber == INDEX)
					{
						pageNumber = POS;
						break;
					}
				}
				if(( mouseX > l3.x ) && ( mouseX < l3.x + l3.w ) && ( mouseY > l3.y ) && ( mouseY < l3.y + l3.h ))
				{
					if(pageNumber == INDEX)
					{
						pageNumber = PERF;
						break;
					}
				}
				if(( mouseX > l4.x ) && ( mouseX < l4.x + l4.w ) && ( mouseY > l4.y ) && ( mouseY < l4.y + l4.h ))
				{
					if(pageNumber == INDEX)
					{
						pageNumber = THRUST_LIM;
						break;
					}
				}
				
				if(( mouseX > l6.x ) && ( mouseX < l6.x + l6.w ) && ( mouseY > l6.y ) && ( mouseY < l6.y + l6.h ))
				{
					if(pageNumber == IDENT)
					{
						pageNumber = INDEX;
						break;
					}
					if(pageNumber == POS)
					{
						pageNumber = INDEX;
						break;
					}
					if(pageNumber == ROUTE)
					{
						pageNumber = INDEX;
						break;
					}
					if(pageNumber == PERF)
					{
						pageNumber = INDEX;
						break;
					}
					if(pageNumber == MAINT)
					{
						pageNumber = INDEX;
						break;
					}
					if(pageNumber == THRUST_LIM)
					{
						pageNumber = INDEX;
						break;
					}
				}
				
				// RIGHT SIDE KEYS
				if(( mouseX > r6.x ) && ( mouseX < r6.x + r6.w ) && ( mouseY > r6.y ) && ( mouseY < r6.y + r6.h ))
				{
					if(pageNumber == IDENT)
					{
						pageNumber = POS;
						break;
					}
					if(pageNumber == INDEX)
					{
						pageNumber = MAINT;
						break;
					}
					if(pageNumber == POS)
					{
						pageNumber = ROUTE;
						break;
					}
					if(pageNumber == PERF)
					{
						pageNumber = THRUST_LIM;
						break;
					}
				}	
			}
		}
	}
}

////////////////GRAPHICS//////////////////
SDL_Surface *loadImage(char *name)
{
	/* Load the image using SDL image */
	SDL_Surface *temp = IMG_Load(name);
	SDL_Surface *image;
	
	if(temp == NULL)
	{
		printf("Failed to load image %s\n", name);
		
		return NULL;
	}
	
	/* Make the background transparent */
	SDL_SetColorKey(temp, (SDL_SRCCOLORKEY|SDL_RLEACCEL), SDL_MapRGB(temp->format, 0, 0, 0));
	
	/* Convert the image to the screen's native format */
	image = SDL_DisplayFormat(temp);
	
	SDL_FreeSurface(temp);
	
	if(image == NULL)
	{
		printf("Failed to convert image %s to native format\n", name);
		
		return NULL;
	}
	
	/* Return the processed image */
	return image;
}

void drawImage(SDL_Surface *srcimage,   int srcX,  int srcY,  int srcW, int srcH,
	       SDL_Surface *destimage, int destX, int destY, int destW, int destH)
{
	SDL_Rect src;
	SDL_Rect dest;
	
	/* Set the src rectangle */
	
	src.x = srcX;
	src.y = srcY;
	src.w = srcW;
	src.h = srcH;
	
	/* Set the blitting rectangle to the size of the src image */
	
	dest.x = destX;
	dest.y = destY;
	dest.w = destW;
	dest.h = destH;
	
	/* Blit the entire image onto the screen at coordinates x and y */
	
	SDL_BlitSurface(srcimage, &src, destimage, &dest);
}

//////////////////LOGIC////////////////////
void doKeys()
{
	/////////////1ST ROW
	if (a.active == 1)
	{
		drawImage(cduImagePressed, 136, 317, 22, 22, screen, 136, 317, 22, 22);
	
		newChar = 'A';
		charCopy(&newChar);
		a.active = 0;

	} else {
		drawImage(cduImage, 136, 317, 22, 22, screen, 136, 317, 22, 22);
	}
	if (b.active == 1)
	{
		drawImage(cduImagePressed, 168, 317, 22, 22, screen, 168, 317, 22, 22);
		
		newChar = 'B';
		charCopy(&newChar);
		b.active = 0;

	} else {
		drawImage(cduImage, 168, 317, 22, 22, screen, 168, 317, 22, 22);
	}
	if (c.active == 1)
	{
		drawImage(cduImagePressed, 200, 317, 22, 22, screen, 200, 317, 22, 22);	
		
		newChar = 'C';
		charCopy(&newChar);
		c.active = 0;

	} else {
		drawImage(cduImage, 200, 317, 22, 22, screen, 200, 317, 22, 22);
	}
	if (d.active == 1)
	{
		drawImage(cduImagePressed, 232, 317, 22, 22, screen, 232, 317, 22, 22);	
	
		newChar = 'D';
		charCopy(&newChar);
		d.active = 0;

	} else {
		drawImage(cduImage, 232, 317, 22, 22, screen, 232, 317, 22, 22);
	}
	if (e.active == 1)
	{
		drawImage(cduImagePressed, 254, 317, 22, 22, screen, 254, 317, 22, 22);	
	
		newChar = 'E';
		charCopy(&newChar);
		e.active = 0;

	} else {
		drawImage(cduImage, 254, 317, 22, 22, screen, 254, 317, 22, 22);
	}
	/////////////2ND ROW
	if (f.active == 1)
	{
		drawImage(cduImagePressed, 136, 349, 22, 22, screen, 136, 349, 22, 22);	
		newChar = 'F';
		charCopy(&newChar);
		f.active = 0;

	} else {
		drawImage(cduImage, 136, 349, 22, 22, screen, 136, 349, 22, 22);
	}
	if (g.active == 1)
	{
		drawImage(cduImagePressed, 168, 349, 22, 22, screen, 168, 349, 22, 22);	
		newChar = 'G';
		charCopy(&newChar);
		g.active = 0;

	} else {
		drawImage(cduImage, 168, 349, 22, 22, screen, 168, 349, 22, 22);
	}
	if (h.active == 1)
	{
		drawImage(cduImagePressed, 200, 349, 22, 22, screen, 200, 349, 22, 22);	
		newChar = 'H';
		charCopy(&newChar);
		h.active = 0;

	} else {
		drawImage(cduImage, 200, 349, 22, 22, screen, 200, 349, 22, 22);
	}
	if (i.active == 1)
	{
		drawImage(cduImagePressed, 232, 349, 22, 22, screen, 232, 349, 22, 22);	
		newChar = 'I';
		charCopy(&newChar);
		i.active = 0;

	} else {
		drawImage(cduImage, 232, 349, 22, 22, screen, 232, 349, 22, 22);
	}
	if (j.active == 1)
	{
		drawImage(cduImagePressed, 254, 349, 22, 22, screen, 254, 349, 22, 22);	
		newChar = 'J';
		charCopy(&newChar);
		j.active = 0;

	} else {
		drawImage(cduImage, 254, 349, 22, 22, screen, 254, 349, 22, 22);
	}
	/////////////3RD ROW
	if (k.active == 1)
	{
		drawImage(cduImagePressed, 136, 381, 22, 22, screen, 136, 381, 22, 22);	
		newChar = 'K';
		charCopy(&newChar);
		k.active = 0;

	} else {
		drawImage(cduImage, 136, 381, 22, 22, screen, 136, 381, 22, 22);
	}
	if (l.active == 1)
	{
		drawImage(cduImagePressed, 168, 381, 22, 22, screen, 168, 381, 22, 22);	
		newChar = 'L';
		charCopy(&newChar);
		l.active = 0;

	} else {
		drawImage(cduImage, 168, 381, 22, 22, screen, 168, 381, 22, 22);
	}
	if (m.active == 1)
	{
		drawImage(cduImagePressed, 200, 381, 22, 22, screen, 200, 381, 22, 22);	
		newChar = 'M';
		charCopy(&newChar);
		m.active = 0;

	} else {
		drawImage(cduImage, 200, 381, 22, 22, screen, 200, 381, 22, 22);
	}
	if (n.active == 1)
	{
		drawImage(cduImagePressed, 232, 381, 22, 22, screen, 232, 381, 22, 22);	
		newChar = 'N';
		charCopy(&newChar);
		n.active = 0;

	} else {
		drawImage(cduImage, 232, 381, 22, 22, screen, 232, 381, 22, 22);
	}
	if (o.active == 1)
	{
		drawImage(cduImagePressed, 254, 381, 22, 22, screen, 254, 381, 22, 22);	
		newChar = 'O';
		charCopy(&newChar);
		o.active = 0;

	} else {
		drawImage(cduImage, 254, 381, 22, 22, screen, 254, 381, 22, 22);
	}
	/////////////4TH ROW
	if (p.active == 1)
	{
		drawImage(cduImagePressed, 136, 413, 22, 22, screen, 136, 413, 22, 22);	
		newChar = 'P';
		charCopy(&newChar);
		p.active = 0;

	} else {
		drawImage(cduImage, 136, 413, 22, 22, screen, 136, 413, 22, 22);
	}
	if (q.active == 1)
	{
		drawImage(cduImagePressed, 168, 413, 22, 22, screen, 168, 413, 22, 22);	
		newChar = 'Q';
		charCopy(&newChar);
		q.active = 0;

	} else {
		drawImage(cduImage, 168, 413, 22, 22, screen, 168, 413, 22, 22);
	}
	if (r.active == 1)
	{
		drawImage(cduImagePressed, 200, 413, 22, 22, screen, 200, 413, 22, 22);	
		newChar = 'R';
		charCopy(&newChar);
		r.active = 0;

	} else {
		drawImage(cduImage, 200, 413, 22, 22, screen, 200, 413, 22, 22);
	}
	if (s.active == 1)
	{
		drawImage(cduImagePressed, 232, 413, 22, 22, screen, 232, 413, 22, 22);	
		newChar = 'S';
		charCopy(&newChar);
		s.active = 0;

	} else {
		drawImage(cduImage, 232, 413, 22, 22, screen, 232, 413, 22, 22);
	}
	if (t.active == 1)
	{
		drawImage(cduImagePressed, 254, 413, 22, 22, screen, 254, 413, 22, 22);	
		newChar = 'T';
		charCopy(&newChar);
		t.active = 0;

	} else {
		drawImage(cduImage, 254, 413, 22, 22, screen, 254, 413, 22, 22);
	}
	/////////////5TH ROW
	if (u.active == 1)
	{
		drawImage(cduImagePressed, 136, 445, 22, 22, screen, 136, 445, 22, 22);	
		newChar = 'U';
		charCopy(&newChar);
		u.active = 0;

	} else {
		drawImage(cduImage, 136, 445, 22, 22, screen, 136, 445, 22, 22);
	}
	if (v.active == 1)
	{
		drawImage(cduImagePressed, 168, 445, 22, 22, screen, 168, 445, 22, 22);	
		newChar = 'V';
		charCopy(&newChar);
		v.active = 0;

	} else {
		drawImage(cduImage, 168, 445, 22, 22, screen, 168, 445, 22, 22);
	}
	if (w.active == 1)
	{
		drawImage(cduImagePressed, 200, 445, 22, 22, screen, 200, 445, 22, 22);	
		newChar = 'W';
		charCopy(&newChar);
		w.active = 0;

	} else {
		drawImage(cduImage, 200, 445, 22, 22, screen, 200, 445, 22, 22);
	}
	if (x.active == 1)
	{
		drawImage(cduImagePressed, 232, 445, 22, 22, screen, 232, 445, 22, 22);	
		newChar = 'X';
		charCopy(&newChar);
		x.active = 0;

	} else {
		drawImage(cduImage, 232, 445, 22, 22, screen, 232, 445, 22, 22);
	}
	if (y.active == 1)
	{
		drawImage(cduImagePressed, 254, 445, 22, 22, screen, 254, 445, 22, 22);	
		newChar = 'Y';
		charCopy(&newChar);
		y.active = 0;

	} else {
		drawImage(cduImage, 254, 445, 22, 22, screen, 254, 445, 22, 22);
	}
	/////////////6TH ROW
	if (z.active == 1)
	{
		drawImage(cduImagePressed, 136, 477, 22, 22, screen, 136, 477, 22, 22);	
		newChar = 'Z';
		charCopy(&newChar);
		z.active = 0;

	} else {
		drawImage(cduImage, 136, 477, 22, 22, screen, 136, 477, 22, 22);
	}
}

void cduPageSelected(int page)
{
	if (pageNumber == IDENT)
	{
		/* MENU NAME */ 
		drawString("IDENT", fontBig, screen, 150, 30, 10, 30);
		/* LEFT KEYS */
		drawString("MODEL", fontSmall, screen, 78, 54, 10, 30);
		drawString("777-200ER", fontBig, screen, 58, 62, 10, 30);
		drawString("NAV DATA", fontSmall, screen, 78, 77, 10, 30);
		drawString("INDEX", fontBig, screen, 58, 190, 10, 30);
		/* RIGHT KEYS */
		drawString("RR Trent 877", fontBig, screen, 176, 62, 10, 30);
		drawString("POS INIT", fontBig, screen, 208, 190, 10, 30);
	}
	if (pageNumber == INDEX)
	{
		/* MENU NAME */
		drawString("INT/REF INDEX", fontBig, screen, 115, 30, 10, 30);
		/* LEFT KEYS */
		drawString("IDENT", fontBig, screen, 58, 62, 10, 30);
		drawString("POS", fontBig, screen, 58, 87, 10, 30);
		drawString("PERF", fontBig, screen, 58, 112, 10, 30);
		drawString("THRUST LIM", fontBig, screen, 58, 138, 10, 30);
		drawString("TAKE OFF", fontBig, screen, 58, 163, 10, 30);
		drawString("APPROACH", fontBig, screen, 58, 190, 10, 30);
		/* RIGHT KEYS */
		drawString("NAV DATA", fontBig, screen, 196, 62, 10, 30);
		drawString("MAINT", fontBig, screen, 223, 190, 10, 30);
	}
	if (pageNumber == POS)
	{
		/* MENU NAME */ 
		drawString("POS INIT", fontBig, screen, 140, 30, 10, 30);
		/* LEFT KEYS */
		drawString("INDEX", fontBig, screen, 58, 190, 10, 30);
		//if (l1.charactercount >= 4){printf("draw inputICAO:\n");}
		drawString(inputICAO, fontBig, screen, 58, 62, 10, 30);
		/* RIGHT KEYS */
		// drawString(inputword, fontBig, screen, 263-(l1.charactercount*8), 62, 10, 30);
		drawString("ROUTE", fontBig, screen, 218, 190, 10, 30);
		/* SCRATCH PAD */
		drawString(inputword, fontSmall, screen, 150, 190, 10, 30);
	}
	if (pageNumber == ROUTE)
	{
		/* MENU NAME */ 
		drawString("RTE 1", fontBig, screen, 150, 30, 10, 30);
		/* LEFT KEYS */
		drawString("ORIGIN", fontSmall, screen, 58, 52, 10, 30);
		drawString("RUNWAY", fontSmall, screen, 58, 77, 10, 30);
		drawString("RTE COPY", fontBig, screen, 58, 163, 10, 30);
		drawString("RTE 2", fontBig, screen, 58, 190, 10, 30);
		/* RIGHT KEYS */
		drawString("1/2", fontSmall, screen, 255, 35, 10, 30);
		drawString("DEST", fontSmall, screen, 248, 50, 10, 30);
		drawString("FLT NO", fontSmall, screen, 237, 79, 10, 30);
		drawString("CO ROUTE", fontSmall, screen, 225, 105, 10, 30);
		drawString("ACTIVATE", fontBig, screen, 199, 190, 10, 30);
	}
	if (pageNumber == PERF)
	{
		/* MENU NAME */ 
		drawString("PERF INIT", fontBig, screen, 150, 30, 10, 30);
		/* LEFT KEYS */
		drawString("GR WT", fontSmall, screen, 58, 52, 10, 30);
		drawString("425.6", fontBig, screen, 58, 62, 10, 30);
		drawString("FUEL", fontSmall, screen, 58, 78, 10, 30);
		drawString(test, fontBig, screen, 58, 87, 10, 30);
		//drawInteger(fuel, fontBig, screen, 58, 87, 10, 30);
		drawString("ZFW", fontSmall, screen, 58, 103, 10, 30);
		drawString("315.0", fontBig, screen, 58, 112, 10, 30);
		drawString("RESERVES", fontSmall, screen, 58, 130, 10, 30);
		drawString("CONST INDEX", fontSmall, screen, 58, 155, 10, 30);
		drawString("INDEX", fontBig, screen, 58, 190, 10, 30);
		/* RIGHT KEYS */
		drawString("CRZ ALT", fontSmall, screen, 232, 52, 10, 30);
		drawString("10000.00", fontBig, screen, 202, 62, 10, 30);
		drawString("CRZ CG", fontSmall, screen, 237, 130, 10, 30);
		drawString("STEP SIZE", fontSmall, screen, 227, 155, 10, 30);
		drawString("THRUST LIM", fontBig, screen, 186, 190, 10, 30);
	}
	if (pageNumber == MAINT)
	{
		/* MENU NAME */ 
		drawString("MAINTENANCE INDEX", fontBig, screen, 90, 30, 10, 30);
		/* LEFT KEYS */
		drawString("CROS LOAD", fontBig, screen, 58, 62, 10, 30);
		drawString("PERF FACTORS", fontBig, screen, 58, 87, 10, 30);
		drawString("IRS MONITOR", fontBig, screen, 58, 112, 10, 30);
		drawString("INDEX", fontBig, screen, 58, 190, 10, 30);
		/* RIGHT KEYS */
		drawString("BITE", fontBig, screen, 235, 62, 10, 30);
	}
	if (pageNumber == THRUST_LIM)
	{
		/* MENU NAME */ 
		drawString("THRUST LIM", fontBig, screen, 120, 30, 10, 30);
		/* LEFT KEYS */
		drawString("SEL", fontSmall, screen, 58, 54, 10, 30);
		drawString("TO", fontBig, screen, 58, 87, 10, 30);
		drawString("TO 1", fontSmall, screen, 58, 105, 10, 30);
		drawString("TO 2", fontSmall, screen, 58, 140, 10, 30);
		drawString("INDEX", fontBig, screen, 58, 190, 10, 30);
		/* MIDDLE KEYS */
		drawString("OAT", fontSmall, screen, 150, 54, 10, 30);
		drawString("20 C", fontBig, screen, 140, 67, 10, 30);
		drawString("<SEL> <ARM>", fontBig, screen, 110, 112, 10, 30);
		
		/* RIGHT KEYS */
		drawString("TO 1 N1", fontSmall, screen, 232, 54, 10, 30);
		drawString("CLB", fontBig, screen, 236, 87, 10, 30);
		drawString("CLB 1", fontBig, screen, 224, 112, 10, 30);
		drawString("CLB 2", fontBig, screen, 224, 138, 10, 30);
		drawString("TAKEOFF", fontBig, screen, 200, 190, 10, 30);
	}
}
////////////////SCREEN UPDATE////////////////
void updateScreen()
{
	/* Blank the screen */
	SDL_FillRect(screen, NULL, 0);
	
	/* Draw the image to 160, 120 */
	drawImage(cduImage, 0, 0, 326, 512, screen, 0, 0, 326, 512);
		
	doKeys();
		
	cduPageSelected(pageNumber);
	
	/* Swap the image buffers */
	SDL_Flip(screen);
	
}


int main(int argc, char *argv[])
{
	int go;
	
	/* Start SDL */
	init("CDU");
	
	/* Call the clean up program when the program exits */
	atexit(cleanup);
	
	go = 1;
	
	cduImage = loadImage("image/CDU.png");
	
	cduImagePressed = loadImage("image/CDU_pushed.png");
	
	/* If we got back NULL image, just exit */
	if(cduImage == NULL)
	{
		exit(0);
	}
	
	initKeys();
	
	fontBig = loadFont("font/DejaVuSans.ttf", 14);
	
	fontSmall = loadFont("font/DejaVuSans.ttf", 8);
		
	/* loop indefinitly for messages */
	while(go == 1)
	{
	/*	if ((csd = SDLNet_TCP_Accept(sd)))
		{			
			quit2 = 0;
			while (!quit2)
			{
				if (SDLNet_TCP_Recv(csd, buffer, 512) > 0)
				{
					char *result;
					int nextVar = 1;
			
					while(nextVar < 6)
					{
						printf("Buffer: %s\n", buffer);
						result = strtok(buffer, ":");
						while (nextVar < 6)//(result != NULL)
						{
							if (nextVar == 1)
							{
								speed = atoi(result);
								//snprintf(test, 16, "%d", speed);
							}
							if (nextVar == 2)
							{
								result = strtok(NULL, ":");
								alt = atoi(result);
							}
							if (nextVar == 3)
							{
								result = strtok(NULL, ":");
								pitch = atoi(result);
							}
							if (nextVar == 4)
							{
								result = strtok(NULL, ":");
								roll = atoi(result);
							}
							if (nextVar == 5)
							{
								result = strtok(NULL, ":");
								fuel = atoi(result);
								snprintf(test, 16, "%d", fuel);
							}
							if (result == NULL)
							{
								exit(1);
							}
							nextVar++;
						}
						//printf("Speed\t| Alt\t| Pitch\t| Roll\t| Fuel\n");
						//printf("%s\t|%d\t|%d\t|%d\t|%s\t\n", speed, alt, pitch, roll, test);
					}*/
					getInput();

					updateScreen();
					
					/* sleep briefly to stop sucking up all of the cpu time */
					SDL_Delay(16);
		/*		}
			}
		}*/
	}

	/* exit the program */
	exit(0);
}
