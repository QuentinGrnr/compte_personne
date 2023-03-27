const sqlite3 = require('sqlite3')
const express = require("express");
const cors = require('cors');
const app = express();


let db = new sqlite3.Database('nbr_personnes.sqlite', err => {
  if (err) throw err
  console.log('data base "nbr_personnes.db" bien active')
})

async function set_api(){
  app.use(cors())
  app.get(`/apiv1/nbrPersonne/:salle`,async (req,res) =>{
    db.all('SELECT * FROM salle_api', async (err, data) => {
      validator = 0
      for (let i = 0; i < data.length; i++) {
        if (data[i].salle === req.params.salle) {
          validator = 1
          db.all('SELECT * FROM ' + req.params.salle + '_nbr_personnes', (err, data_) => {
            res.json({
              salle: req.params.salle,
              adrs_mac: data[i].adrs_mac,
              stats: data_
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
  app.get(`/apiv1/salle`,async (req,res) =>{
    db.all('SELECT * FROM salle_api', async (err, data) => {
      res.json({
        salle: data
      })
    })
  })
}

set_api()

app.listen(500,() => console.log('listening on port 500'))
