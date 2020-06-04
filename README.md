# AutoMunch

## What
AutoMunch is an IoT device that is placed inside a food container. It will monitor the level of food inside the container and when it's running low, an SMS alert will be sent.

AutoMunch will log your food level data in a spreadsheet. With this data, intelligent predictions can be created that will inform you how often you will need to restock and the average costs associated with this.

## How
ESP8266 with wifi capabilities stores the data in a spreadsheet and creates a graph so that a data story can be presented.

VL53L0X monitors the range inside the food container, the further the distance means the lower the food supply is.

Once the distance is great enough, an SMS alert will be sent to the user informing them that their food levels are running low.

A data visual presents a graph showing the usage trends over time, as well as the minimum price found on Amazon and the average days between low stock.

## Future Implementations
UI for the user to be able to select the product they are monitoring so it can be searched for online when levels are running low.

Predictions when the product will be finished and an automated order so it can be replenished at the right time.

Place orders via the SMS alerts.
