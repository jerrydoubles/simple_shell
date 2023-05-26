#include <stdio.h>

/**
 * main - Entry point of the program
 *
 * Description: This function calculates the total cost of a purchase based on
 * the number of items purchased and the price per item.
 *
 * Return: 0 on success
 */
int main(void)
{
	int number_of_items;
	float price_per_item;
	float total_cost;

	printf("Enter the number of items: ");
	scanf("%d", &number_of_items);

	printf("Enter the price per item: ");
	scanf("%f", &price_per_item);

	total_cost = number_of_items * price_per_item;

	printf("The total cost is %.2f\n", total_cost);

	return (0);
}