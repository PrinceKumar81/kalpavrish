#include<stdio.h>
#include<stdlib.h>
#include<string.h>
// All attribute Defined here
typedef struct {
    int product_id;
    char product_name[60];
    float price;
    int quantity;
}product;
// Functions
void addproduct(product **inventory,int *count);
void viewallproduct(product *inventory,int count);
void updateQuantity(product *inventory,int count);
void searchbyId(product *inventory,int count);
void searchByName(product *inventory, int count);
void searchByPriceRange(product *inventory, int count);
void deleteProduct(product **inventory, int *count);
void displayMenu();
void clearInputBuffer();
int isUniqueId(product *inventory, int count, int id); 
int main()
{
    product *inventory = NULL;
    int count=0;
    int choice;
    //Step1->asking number of Product from User
    printf("Enter initial number of product ");
    scanf("%d",&count);
    clearInputBuffer();

    inventory = (product *)calloc(count,sizeof(product));
    if(inventory == NULL)
    {
        printf("memory allocation failed,please try again");
        return -1;
    }
    for(int i=0;i<count;i++)
    {
        printf("\nEnter the details of Product\n",i+1);
        int id;
        while(1)
        {
            printf("Product ID : ");
            scanf("%d",&id);
            clearInputBuffer();

            if(id<=0)
            {
                printf("Product Id Must be Greater than zero\n");
            } else if(!isUniqueId(inventory,i,id))
            {
                printf("Product Id already Exists,Try another Product Id\n");
            }else
            {
                inventory[i].product_id=id;
                break;
            }
        }
        printf("Product Name :");
        fgets(inventory[i].product_name,60,stdin);
        inventory[i].product_name[strcspn(inventory[i].product_name,"\n")]='\0';

        while(1){
            printf("Product price : ");
            scanf("%f",&inventory[i].price);
            if(inventory[i].price==0)
            {
                printf("Price cannot be zero of any Product,Re-enter it");
            }
            else if(inventory[i].price<0)
            {
                 printf("Price cannot be less than zero of any Product,Re-enter it\n");
            }
            else
            {
                break;
            }
        }
        while(1)
        {
            printf("Product Quantity:");
            scanf("%d",&inventory[i].quantity);
            if(inventory[i].quantity==0)
            {
                printf("Quantity cannot be zero,Try again\n");
            }
            else if(inventory[i].quantity<0)
            {
                printf("Quantity cannot be less than Zero,Try again");
            }
            else
            {
                break;
            }
        }
    }
    while(1)
    {
        displayMenu();
        printf("Enter Your choice ");
        scanf("%d",&choice);
        clearInputBuffer();
        switch(choice)
        {
            case 1:
            addproduct(&inventory,&count);
            break;
            case 2:
            viewallproduct(inventory,count);
            break;
            case 3:
            updateQuantity(inventory,count);
            break;
            case 4:
            searchbyId(inventory,count);
            break;
            case 5:
            searchByName(inventory,count);
            break;
            case 6:
            searchByPriceRange(inventory,count);
            break;
            case 7:
            deleteProduct(&inventory,&count);
            break;
            case 8:
            printf("\nMemeory Released Successfully");
            free(inventory);
            return 0;
            default :
            printf("Invalid choice ,Again try it");
        }
    }
    return 0;
}
void displayMenu()
{
    printf("\n=======Inventory Menu ========\n");
    printf("1. Add New Product\n");
    printf("2. View All Products\n");
    printf("3. Update Quantity\n");
    printf("4. Search Product by ID\n");
    printf("5. Search Product by Name\n");
    printf("6. Search Product by Price Range\n");
    printf("7. Delete Product\n");
    printf("8. Exit\n");
}
void clearInputBuffer()
{
    int c;
    while((c=getchar())!='\n' && c!=EOF);
}
int isUniqueId(product *inventory, int count, int id)
{
    for (int i = 0; i < count; i++) {
        if (inventory[i].product_id == id) {
            return 0;
        }
    }
    return 1;
}
void addproduct(product **inventory ,int *count)
{
    printf("\nEnter new Product Details:\n");
    *inventory = (product *)realloc(*inventory, (*count + 1) * sizeof(product));
    if (*inventory == NULL) {
        printf("Memory reallocation failed!\n");
        return;
    }
    int id;
    while (1) {
        printf("Product ID: ");
        scanf("%d", &id);
        clearInputBuffer();
        if (id <= 0) {
            printf("Product ID must be greater than zero. Try again.\n");
        } else if (!isUniqueId(*inventory, *count, id)) {
            printf("Product ID already exists. Please enter a different ID.\n");
        } else {
            (*inventory)[*count].product_id = id;
            break;
        }
    }
    while (1) {
        printf("Product Name: ");
        fgets((*inventory)[*count].product_name, 60, stdin);
        (*inventory)[*count].product_name[strcspn((*inventory)[*count].product_name, "\n")] = '\0';

        if (strlen((*inventory)[*count].product_name) == 0) {
            printf("Product name cannot be empty. Try again.\n");
        } else {
            break;
        }
    }
    while (1) {
        printf("Product Price: ");
        scanf("%f", &(*inventory)[*count].price);
        clearInputBuffer();

        if ((*inventory)[*count].price == 0) {
            printf("Price cannot be zero. Re-enter.\n");
        } else if ((*inventory)[*count].price < 0) {
            printf("Price cannot be less than zero. Re-enter.\n");
        } else {
            break;
        }
    }
    while (1) {
        printf("Product Quantity: ");
        scanf("%d", &(*inventory)[*count].quantity);
        clearInputBuffer();

        if ((*inventory)[*count].quantity == 0) {
            printf("Quantity cannot be zero. Try again.\n");
        } else if ((*inventory)[*count].quantity < 0) {
            printf("Quantity cannot be less than zero. Try again.\n");
        } else {
            break;
        }
    }

    (*count)++;
    printf("Product added successfully!\n");
}
void viewallproduct(product *inventory,int count)
{
    printf("\n======Product List =======\n");
    if(count == 0)
    {
        printf("No Product available to Display \n");
        return;
    }
    for(int i=0;i<count;i++)
    {
        printf("product ID :%d || Name %s || Price %2f || Quantity %d\n",inventory[i].product_id,inventory[i].product_name,inventory[i].price,inventory[i].quantity);
    }
}
void updateQuantity(product *inventory,int count)
{
    int id,newQuantity;
    int found=0;

    printf("\nEnter prosuct Id to update Quantity\n");
    scanf("%d",&id);
    clearInputBuffer();
    for(int i=0;i<count;i++)
    {
        if(inventory[i].product_id==id)
        {
            printf("Enter new Quantity: ");
            scanf("%d",&newQuantity);
            clearInputBuffer();
            inventory[i].quantity=newQuantity;
            printf("Quantity updated Successfully\n");
            found=1;
            break;
        }
    }
    if(!found)
    {
        printf("Product with Id %d not found",id);
    }
}
void searchbyId(product *inventory,int count)
{
    int id;
    int found = 0;
    printf("\nEnter produxt Id to search:\n");
    scanf("%d",&id);
    clearInputBuffer();

    for(int i=0;i<count;i++)
    {
        if(inventory[i].product_id==id)
        {
            printf("Product found:\n || Product ID :%d || Name: %s || price: %.2f || Quantity :%d\n",
            inventory[i].product_id,
            inventory[i].product_name,
            inventory[i].price,
            inventory[i].quantity
            );
            found =1;
            break;
        }
    }
    if(!found)
    {
        printf("product with ID %d not found \n",id);
    }
}
//adding to make case-insensitive
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] = str[i] + ('a' - 'A');
    }
}
void searchByName(product *inventory,int count)
{
    char searchName[60];
    int found=0;

    printf("\nEnter the Name to Search ");
    fgets(searchName,60,stdin);
    searchName[strcspn(searchName,"\n")]='\0';
    toLowerCase(searchName);

    printf("Product Found\n");
    for(int i=0;i<count;i++)
    {
        if(strstr(inventory[i].product_name,searchName)!=NULL)
        {
            printf("Product Id:\n Name:%s || Price %.2f || Quantity %d\n",
            inventory[i].product_id,
            inventory[i].product_name,
            inventory[i].price,
            inventory[i].quantity
            );
            found = 1;
        }
    }
    if(!found)
    {
        printf("No product Found with this name%s ",searchName);
    }
}
void searchByPriceRange(product *inventory, int count) {
    float minPrice, maxPrice;
    int found = 0;
    
    printf("\nEnter minimum price: ");
    scanf("%f", &minPrice);
    
    printf("Enter maximum price: ");
    scanf("%f", &maxPrice);
    clearInputBuffer();
    
    printf("\nProducts in price range:\n");
    for (int i = 0; i < count; i++) {
        if (inventory[i].price >= minPrice && inventory[i].price <= maxPrice) {
            printf("Product ID: %d | Name: %s | Price: %.2f | Quantity: %d\n",
                   inventory[i].product_id,
                   inventory[i].product_name,
                   inventory[i].price,
                   inventory[i].quantity);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No products found in the price range %.2f - %.2f\n", minPrice, maxPrice);
    }
}
void deleteProduct(product **inventory, int *count) {
    int id;
    int found = -1;
    printf("\nEnter Product ID to delete: ");
    scanf("%d", &id);
    clearInputBuffer();
    for (int i = 0; i < *count; i++) {
        if ((*inventory)[i].product_id == id) {
            found = i;
            break;
        }
    }
    
    if (found == -1) {
        printf("Product with ID %d not found!\n", id);
        return;
    }
    for (int i = found; i < *count - 1; i++) {
        (*inventory)[i] = (*inventory)[i + 1];
    }
    (*count)--;
    if (*count > 0) {
        *inventory = (product *)realloc(*inventory, (*count) * sizeof(product));
        if (*inventory == NULL && *count > 0) {
            printf("Memory reallocation failed!\n");
            return;
        }
    } else {
        free(*inventory);
        *inventory = NULL;
    }
    printf("Product deleted successfully!\n");
}