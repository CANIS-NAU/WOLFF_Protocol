#ifndef ETSYDECODECLIENTMESSAGE_H_INCLUDED
#define ETSYDECODECLIENTMESSAGE_H_INCLUDED
#include "ApplicationMapping.h"
#include "etsy_service_mapping.h"


// This code is only for debugging and demo purpose.
// Must modify before actual deployment.
void DecodeEtsyClientMessage(char* encodedPayLoad,int payLoadLength)
{
    if(payLoadLength!=13)
        return;
    else
    {
            char* tempPtr = encodedPayLoad; // tempPtr pointing to index 0

            Application app = (unsigned char)(*(tempPtr));

            if(app==Etsy)
            {
                printf("Application: Etsy\n\n");

                tempPtr++; //tempPtr pointing to index 1
                EtsyService es = (unsigned char)(*(tempPtr));

                if(es==CreateListing)
                {
                    printf("Service: Create Listing\n\n");

                    tempPtr++; // tempPtr pointing to index 2
                    Title t = (unsigned char)(*(tempPtr));

                    if(t==painting)
                    {
                        printf("Title: Painting\n\n");
                    }
                    else if(t==bracelet)
                    {
                        printf("Title: Bracelet\n\n");
                    }
                    else if(t==necklace)
                    {
                        printf("Title: Necklace\n\n");
                    }
                    else if(t==poncho)
                    {
                        printf("Title: Poncho\n\n");
                    }
                    else if(t==tapestry)
                    {
                        printf("Title: Tapestry\n\n");
                    }
                    else
                    {
                        printf("Title: Invalid Title\n\n");
                    }

                    tempPtr++; // tempPtr pointing to index 3
                    Description d = (unsigned char)(*(tempPtr));

                    if(d==handmade) // handmade,oil,watercolor,woven,invalid_desc
                    {
                        printf("Description: Handmade\n\n");
                    }
                    else if(d==oil)
                    {
                        printf("Description: Oil Medium\n\n");
                    }
                    else if(d==watercolor)
                    {
                        printf("Description: Watercolor Medium\n\n");
                    }
                    else if(d==woven)
                    {
                        printf("Description: Woven\n\n");
                    }
                    else
                    {
                        printf("Invalid Description\n\n");
                    }

                    tempPtr++; // tempPtr pointing to index 4
                    int quantity = (unsigned char)(*(tempPtr));

                    printf("Quantity: %d\n\n",quantity);

                    tempPtr++; // tempPtr pointing to index 5
                    int price_LM_8_SB = (unsigned char)(*(tempPtr));


                    tempPtr++;// tempPtr now pointing to index 6
                    unsigned char indx6value = (unsigned char)(*(tempPtr));
                    int price_RM_4_SB = indx6value>>4; // Shifting 4 spaces towards right to get the desired value

                    unsigned char maskValue = 0b00001111;
                    indx6value = indx6value & maskValue; // Bitwise AND operation
                    int price_fraction_portion = indx6value;


                    double price = (price_LM_8_SB * 16) + (price_RM_4_SB) + ((double)price_fraction_portion/100.0);

                    printf("Price: %lf\n\n", price);


                    tempPtr++; // tempPtr pointing to index 7
                    unsigned char indx7value = (unsigned char)(*(tempPtr));

                    WhoMade whmd = indx7value>>4;
                    if(whmd==i)
                    {
                        printf("Who Made: I made\n\n");
                    }
                    else if(whmd==we)
                    {
                        printf("Who Made: We made\n\n");
                    }
                    else if(whmd==other)
                    {
                        printf("Who Made: Others made\n\n");
                    }
                    else
                    {
                        printf("Invalid Maker\n\n");
                    }


                    maskValue = 0b00001110; // Extracting bits 3 to 1
                    WhenMade wnmd = ((indx7value & maskValue) >>1);

                    if(wnmd==made_to_order)
                    {
                        printf("When Made: Made to order\n\n");
                    }
                    else if(wnmd==made_to_sell)
                    {
                        printf("When Made: Made to sell\n\n");
                    }
                    else if(wnmd==made_to_special)
                    {
                        printf("When Made: Made to special\n\n");
                    }
                    else
                    {
                        printf("Invalid Value for When_Made\n\n");
                    }


                    tempPtr++; // tempPtr now pointing to index 8
                    long int part5 = (unsigned char)(*(tempPtr));

                    tempPtr++; // tempPtr now pointing to index 9
                    long int part4 = (unsigned char)(*(tempPtr));

                    tempPtr++; // tempPtr now pointing to index 10
                    long int part3 = (unsigned char)(*(tempPtr));

                    tempPtr++; // tempPtr now pointing to index 11
                    long int part2 = (unsigned char)(*(tempPtr));

                    tempPtr++; // tempPtr now pointing to index 12
                    long int part1 = (unsigned char)(*(tempPtr));


                    unsigned long int shippingTemplate = (part5*256*256*256*256)
                    +(part4*256*256*256)+(part3*256*256)+(part2*256)+part1;

                    printf("Shipping Template ID: %lu\n\n", shippingTemplate);

                }

            }
    }
}
#endif // ETSYDECODECLIENTMESSAGE_H_INCLUDED
