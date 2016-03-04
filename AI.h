#ifndef H_AI
#define H_AI

// CONSTANTS ============================================================
#define IA_V3_MAX_PICK 20 /** \def IA_V3_MAX_PICK maximum times that the IA tries
								   picking a random direction before giving up.
								   Used to avoid infinite picking.*/

// PROTOTYPES ==========================================================
// Helpers =============================================================
int detect(snake* s, direction c, field* map);

// AI main functions ===================================================
direction rngesus(snake* s);
direction rngesus2(snake* s, field* map);
direction AI_random_v3(snake* s, field* map);

#endif
