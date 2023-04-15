const sqlite3 = require('sqlite3')
const express = require("express");
const cors = require('cors');
const app = express();


let db = new sqlite3.Database('nbr_personnes.sqlite', err => {
  if (err) throw err
  console.log('data base "nbr_personnes.db" bien active')
})

async function set_api(){
  app.use(cors()) // permet de faire des requetes depuis n'importe quel site
  app.get(`/apiv1/nbrPersonne/:salle`,async (req,res) =>{ // on récupère les données de la salle mise en paramètre
    db.all('SELECT * FROM salle_api', async (err, data) => { // on récupère les données de la table => les salles
      validator = 0 // on initialise le validateur à 0
      for (let i = 0; i < data.length; i++) { // on parcourt les salles
        if (data[i].salle === req.params.salle) { // on compare les salles avec la salle demandée
          validator = 1 // si la salle existe on passe le validateur à 1
          db.all('SELECT * FROM ' + req.params.salle + '_nbr_personnes', (err, data_) => { // on récupère les données de la salle
            res.json({ // on renvoie les données
              salle: req.params.salle, // on renvoie le nom de la salle
              adrs_mac: data[i].adrs_mac, //  on renvoie l'adresse mac du module bluetooth
              capacite : data[i].capacite, // on renvoie la capacité de la salle
              stats: data_ // on renvoie les données de la salle
            })
          })
        }
      }
      if (validator==0){
        res.json({
          message: "Cette salle n'est pas encore en place dans notre api."
        })
      }
    })
  })
  app.get(`/apiv1/salle`,async (req,res) =>{ // on récupère les données des salles
    db.all('SELECT * FROM salle_api', async (err, data) => { // on récupère les données de la table => les salles
      res.json({
        salle: data //  on renvoie les données des salles
      })
    })
  })
}

set_api()

app.listen(500,() => console.log('listening on port 500'))
