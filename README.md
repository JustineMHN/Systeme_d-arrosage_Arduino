# Projet de Déclenchement d'un Arrosage à Distance en Fonction de la Température Reçue

## Introduction

Ce document décrit le projet visant à déclencher un système d'arrosage à distance en fonction des variations de température. Le projet utilise un Arduino équipé d'un module WiFi pour recevoir des informations de température d'une source externe(Arduino distant). En fonction de ces données, le système contrôle un servomoteur pour déclencher l'arrosage et utilise des LED pour indiquer l'état du processus.

## Matériel Requis

1. Arduino avec module WiFi
2. Capteur de température externe
3. Servomoteur pour la vanne d'arrosage
4. Afficheur LCD I2C
5. LED de différentes couleurs (rouge, jaune, vert)
6. Câblage approprié

## Librairies Utilisées

1. WiFiEsp
2. WiFiEspUdp
3. LiquidCrystal_I2C
4. Servo

## Fonctionnalités Principales

### 1. Configuration WiFi

Le code configure le module WiFi avec les informations réseau pour permettre la communication à distance.

```cpp
char ssid[] = "Nom_du_Réseau";
char pass[] = "Mot_de_Passe";
```

### 2. Initialisation des Composants

Les composants tels que l'afficheur LCD, les LED (rouge, jaune, vert), le servomoteur, et le capteur de température externe sont correctement initialisés dans la fonction `setup()`.

### 3. Communication à Distance

Le code utilise la communication WiFi pour recevoir les données de température d'une source externe. Il crée un serveur UDP sur le port `2390` et écoute les messages.

### 4. Gestion des Messages Reçus

Lorsqu'un message est reçu, le code analyse la température et déclenche l'arrosage en fonction des seuils définis. Les LED indiquent l'état du processus.

### 5. Affichage sur l'Écran LCD

L'afficheur LCD est mis à jour avec le décompte du temps de l'arrosage et la température reçue.

### 6. Contrôle du Servomoteur pour l'Arrosage

En fonction de la température, le servomoteur contrôle l'ouverture/fermeture de la vanne d'arrosage(le servomoteur dans notre cas).

## Fonctionnement

Le programme surveille constamment les messages de température reçus par le module WiFi. En fonction de ces données, il ajuste le système d'arrosage en conséquence. Les LED fournissent un retour visuel sur l'état actuel du processus, tandis que l'afficheur LCD affiche des informations détaillées.

## Conclusion

Ce projet démontre l'utilisation pratique du module WiFi Arduino pour créer un système intelligent de déclenchement d'arrosage à distance en fonction des conditions météorologiques.
