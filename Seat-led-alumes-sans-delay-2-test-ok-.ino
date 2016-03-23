/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actionneur d’ascenseur a barrière infrarouge.ino   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juthomas <juthomas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/10/17 16:15:44 by juthomas          #+#    #+#             */
/*   Updated: 2015/10/18 02:09:47 by juthomas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// fichier sans delay sur positions allumees et eteintes des leds, leds allumes de base

#include <Servo.h>

Servo myservo1;  // declaration des servos (ty Servo.h to make that izi)
Servo myservo2;

int photodiode[2] = {0, 1}; // met les photodiodes sur les entrees 0 et 1
int val[2] = {0, 0};    // initialisation du tableau
int led[2] = {5, 6};
int tempspulse = 1000;// temps d'une pulsation (en ms) reglable sera par la suite couplee avec potentiomettre
int nombre_de_position_leds = 30; // precision lumineuse en nb de position entre min et max ; 20 opti, si augmenté clignotement visible (a partir de 30)


int racine2(int nb)
{
  int a;

  a = 0;
  while (a < nb)
  {
    a++;
     if ((a * a) == nb)
      return (a);
  }
  return (0);
}

int racine1(int nb)
{
  while (racine2(nb) == 0)
    nb++;
  nb = racine2(nb);
  return (nb);
}



void setup()
{
  pinMode(led[0], OUTPUT); // met les leds sur broches 6 et 7
  pinMode(led[1], OUTPUT);
  myservo1.attach(9);  // met les servos sur broches 8 et 9 sortie
  myservo2.attach(8);
}

void  pulse(int i, int del)    // revoir ca car si del trop grand ca clignote comme de la merde
{
  int re;
  if (i <= del)
  {
    re = map(i, 0, del, 0, nombre_de_position_leds); 
  if (val[0] == 1)
    analogWrite(led[0], 0);
  else if (val[1] == 1)
    analogWrite(led[1], 0);
   delay(re);
   if (val[0] == 1)
    analogWrite(led[0], 1023);
  else if (val[1] == 1)
    analogWrite(led[1], 1023);
   delay(nombre_de_position_leds - re);
  pulse(i + 1, del);
  }
  if (val[0] == 1)
    analogWrite(led[0], 0);
  else if (val[1] == 1)
    analogWrite(led[1], 0);
   delay(re);
   if (val[0] == 1)
    analogWrite(led[0], 1023);
  else if (val[1] == 1)
    analogWrite(led[1], 1023);
   delay(nombre_de_position_leds - re);
}

void  pulse1(int i, int del)    // revoir ca car si del trop grand ca clignote comme de la merde
{
 int re;

  if (i <= del)
  {
    
   re = map(i, 0, del, 0, 255);
   if (val[0] == 1)
    analogWrite(led[0], 255 - re);
  else if (val[1] == 1)
    analogWrite(led[1], 255 - re);
  delay(nombre_de_position_leds);
  pulse1(i + 1, del);
  }
   if (val[0] == 1)
    analogWrite(led[0], 255 - re);
  else if (val[1] == 1)
    analogWrite(led[1], 255 - re);
   delay(nombre_de_position_leds);
}

void attribute(int uni)
{
  //int i = racine1(tempspulse);
  int i = tempspulse / nombre_de_position_leds;
  int nbpulses = 20;
  
 while (uni < 125)
 {
    if (val[0] == 1)
      myservo1.write(uni);
    else if(val[1] == 1) // delete if maybe
      myservo2.write(uni);
    delay(15);
    uni++;
 }
  delay(500);
 while (uni > 45)
 {
    if (val[0] == 1)
      myservo1.write(uni);
    else if(val[1] == 1) // delete if maybe
      myservo2.write(uni);
    delay(15);
    uni--;
 }
 while (nbpulses > 0)
 {
    pulse1(0, i);
  //  delay(2000);
    nbpulses--;
 }
}

void loop()
{
  myservo1.write(45);
  myservo2.write(45);
  analogWrite(led[0], 1023);
  analogWrite(led[1], 1023);
  val[0] = analogRead(photodiode[0]);
  val[1] = analogRead(photodiode[1]);
// val[0] = map(val[0], 20, 100, 0, 1);     // min : 30, max : 800, plus que moitier: 1, moins: 0 
//  val[1] = map(val[1], 20, 100, 0, 1);
//  val[0] = 30;
//  val[1] = 60; 
  if (val[0] < 40 || val[1] < 40)
  {
    if (val[0] < 40)
    {
      val[0] = 1;
      val[1] = 0;
    }
    else if (val[1] < 40)
    {
      val[0] = 0;
      val[1] = 1;  
    }
    attribute(45);
  }
//  if (val[0] == 1 || val[1] == 1)
//    attribute(35);
  delay(15);
}

