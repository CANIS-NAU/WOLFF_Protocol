#ifndef ETSY_SERVICE_MAPPING_H_INCLUDED
#define ETSY_SERVICE_MAPPING_H_INCLUDED


typedef enum EtsyService {CreateListing, Gubo,Invalid_Service}
EtsyService;

// Description is the borader category of items (1 byte)
typedef enum Description
{handmade,oil,watercolor,woven,invalid_desc}
Description;

//Title is the more focused sub-category of items (1 byte)
typedef enum Title
{painting,bracelet,necklace,poncho,tapestry,invalid_title}
Title;

typedef enum WhoMade // Assuming it means the creator of the item
{i,we,other,invalid_maker}
WhoMade;

typedef enum WhenMade
{made_to_order,made_to_sell,made_to_special,invalid_when_made}
WhenMade;


#endif
