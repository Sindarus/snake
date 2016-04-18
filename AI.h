#ifndef H_AI
#define H_AI

// CONSTANTS ============================================================
#define IA_MAX_PICK 20 /** \def IA_MAX_PICK maximum times that the IA tries
							   picking a random direction before giving up.
							   Used to avoid infinite picking.*/

// PROTOTYPES ==========================================================
// Helpers =============================================================
int detect(snake* s, direction c, field* map);
bool not_in(coord c, coord* tableau, int taille);

// AI main functions ===================================================
direction rngesus(snake* s);
direction rngesus2(snake* s, field* map);
direction spread(snake* s,field* map);


#endif
