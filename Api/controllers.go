package main

import (
	"context"
	"fmt"
	"github.com/labstack/echo"
	"go.mongodb.org/mongo-driver/bson"
	"log"
	"net/http"
	"time"
)

type Apagador struct {
	IdDispositivo     string    `json:"id_dispositivo"`
	NombreDispositivo string    `json:"nombre_dispositivo"`
	Valor1            string    `json:"valor_1"`
	Valor2            string    `json:"valor_2"`
	UltAct            time.Time `json:"ult_act"`
}

var apagadorCollection = db().Database("Prueba").Collection("apagador")
var ctx = context.Background()

func guardarData(c echo.Context) error {
	d:= new(Apagador)
	if err:= c.Bind(d); err != nil {
		return err
	}
	d.UltAct = time.Now()

	insertResult, err := apagadorCollection.InsertOne(ctx, d)
	if err != nil {
		log.Fatal(err)
	}

	return c.JSON(http.StatusCreated, insertResult.InsertedID)
}

func obtenerData(c echo.Context) error {
	cursor, err := apagadorCollection.Find(ctx, bson.M{})
	if err != nil {
		fmt.Println(err)
	}

	apagadores := []Apagador{}
	for cursor.Next(ctx) {
		var p Apagador
		if err := cursor.Decode(&p); err != nil {
			log.Fatal("cursor. Decode ERROR:", err)
		}
		apagadores = append(apagadores, p)
	}

	return c.JSON(http.StatusOK, apagadores)
}