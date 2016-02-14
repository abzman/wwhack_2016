# Pinouts #

## Motors ##

### Main Drive ##
| Wire  | Function          |
|:------|:------------------|
| Blue  | (-) for forward   |
| Brown | (+) for forward   |

### Steering ##
| Wire  | Function          |
|:------|:------------------|
| Blue  | (-) for right     |
| Brown | (+) for right     |

### LIDAR Servo ###
| Wire  | Function          |
|:------|:------------------|
| Yellow| Signal            |
| Orange| 4.8-6VDC          |
| Brown | Ground            |

## Sensors ##

### LIDAR ###
| Pin   | Function |
|:------|:---------|
| 1     | VCC      |
| 2     | NC       |
| 3     | NC       |
| 4     | SCL      |
| 5     | SDA      |
| 6     | Ground   |

### Sonar ###
| Pin   | Function | UNO    | Wire          |
|:------|:---------|:-------|:--------------|
| 1     | VCC      | VCC    | Blue          |
| 2     | Trig     | D12    | Blue/Black    |
| 3     | Echo     | D11    | Green         |
| 4     | NC       | NC     |               |
| 5     | Ground   | Ground | Green/Black   |

### DHT22 ###
| Pin   | Function | UNO    | Wire      |
|:------|:---------|:-------|:----------|
| 1     | VCC      | VCC    | Grey      |
| 2     | Trig     | D13    | White     |
| 3     | Ground   | Ground | Black     |
| 4     | Ground   | Ground | NC        |

### Lux ###
| Pin   | Function |
|:------|:---------|
| 1     | VCC      |
| 2     | SCL      |
| 3     | SDA      |
| 4     | NC       |
| 5     | Ground   |

### BMP180 ###
| Pin   | Function |
|:------|:---------|
| 1     | VCC      |
| 2     | Ground   |
| 3     | SCL      |
| 4     | SDA      |

### I2C Host ###
| Pin   | Function | Wire       |
|:------|:---------|:-----------|
| 1     | GND      | Red/Black  |
| 2     | VCC      | Red        |
| 3     | SDA      | Brown      |
| 4     | SCL      | Brown/Black|


# I2C Addresses #

| Address   | Device    |
|:----------|:----------|
| 0x62      | LIDAR     |
| 0x23      | Lux       |
| 0x77      | Baro      |