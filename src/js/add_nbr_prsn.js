//note = pour simplifier la chose => creer un tableau avec tout les ip qui doivent etre dans le graphique
const sqlite3 = require('sqlite3')
const fetch = require('node-fetch');
const { text } = require('express');

let db = new sqlite3.Database('nbr_personnes.sqlite', err => {
  console.log(err)
    if (err) throw err
    console.log('data base "nbr_personnes.sqlite" bien active')
  })

async function add_salle_api(salle){
  db.run('CREATE TABLE IF NOT EXISTS salle_api(id INTEGER PRIMARY KEY AUTOINCREMENT,salle VARCHAR)')
  db.all('SELECT * FROM salle_api', async (err, data) => {
    validator = 0
    for (let i = 0; i < data.length; i++) {
      if(data[i].salle === salle) {
        console.log("Cette salle est déjà en place dans l'api")
        validator = 1
      }
    }
    if(validator==0){
      db.run('INSERT INTO salle_api(salle) VALUES(?)', salle);
      db.run('CREATE TABLE ' + salle + '_nbr_personnes(id INTEGER PRIMARY KEY AUTOINCREMENT,hour VARCHAR, nbr_personnes BLOB)')
      db.all('SELECT * FROM salle_api', async (err, data) => {
      })
    }
    else {
      return 0 
    }
  })
}
add_salle_api("a0-20")

async function getdata_salle(){
  let minute = new Date().getMinutes();
if (minute % 5 === 0) {
  db.all('SELECT * FROM salle_api', async (err, data) => {
    for (let i = 0; i < data.length; i++) {
      await add_players_data(data[i].name,data[i].ip)
    }
  })
  setInterval(() => {
    db.all('SELECT * FROM salle_api', async (err, data) => {
      for (let i = 0; i < data.length; i++) {
        await add_players_data(data[i].salle)
      }
    })
  }, 300000);
} else {
  setTimeout(getdata_salle,30000)
}
}


async function add_players_data(salle){
  // modif ce code avec le programme bluetooth qui renvoie les données
  /*
    await fetch(
      `https://api.mcstatus.io/v2/status/bedrock/` + ip)
      .then(res => res.json())
      .then( async (text) => {
        if(text.online == true){
          db.all('SELECT COUNT(*) FROM  ' + name + '_players_online', async (err, data1) => {
            if (err) {
                console.log(err)
            } else {
                let count = data1[0]['COUNT(*)'];
                if (count < 288){
                  db.run('INSERT INTO ' + name + '_players_online(hour,players) VALUES(?,?)', [ await getDateFormatted(), text.players.online]);
                  db.all('SELECT * FROM ' + name + '_players_online', (err, data) => {
                    console.log(data)
                    if (err)
                      throw err
                  })
                }else{
                  db.run('DELETE FROM ' + name + '_players_online WHERE id = (SELECT MIN(id) FROM ' + name + '_players_online)')
                  db.run('INSERT INTO ' + name + '_players_online(hour,players) VALUES(?,?)', [ await getDateFormatted(), text.players.online]);
                  db.all('SELECT * FROM ' + name + '_players_online', (err, data) => {
                    console.log(data)
                    if (err)
                      throw err
                  })
                }
        }
      });
        }
        else{
          db.all('SELECT COUNT(*) FROM  ' + name + '_players_online', async (err, data1) => {
            if (err) {
                console.log(err)
            } else {
                let count = data1[0]['COUNT(*)'];
                if (count < 288){
                  db.run('INSERT INTO ' + name + '_players_online(hour,players) VALUES(?,?)', [ await getDateFormatted(), 0]);
                  db.all('SELECT * FROM ' + name + '_players_online', (err, data) => {
                    console.log(data)
                    if (err)
                      throw err
                  })
                }else{
                  db.run('DELETE FROM ' + name + '_players_online WHERE id = (SELECT MIN(id) FROM ' + name + '_players_online)')
                  db.run('INSERT INTO ' + name + '_players_online(hour,players) VALUES(?,?)', [ await getDateFormatted(), 0]);
                  db.all('SELECT * FROM ' + name + '_players_online', (err, data) => {
                    console.log(data)
                    if (err)
                      throw err
                  })
                }
        }
      });
        }
    })*/
}

function getDateFormatted() {
  var now = new Date();
  var hour = (now.getHours() < 10 ? "0" + now.getHours() : now.getHours())
  var minutes = (now.getMinutes() < 10 ? "0" + now.getMinutes() : now.getMinutes())
  return hour + ":" + minutes;
}

getdata_ip()
