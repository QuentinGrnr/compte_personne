async function build_chart_player_online(){
  const hour_canvas = await document.getElementById("hours_affluence");
  const hour_affluence_chart = await new Chart(hour_canvas,{
    type:"bar",
    data:{
      labels: [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23],
      datasets : [{
        label: "Affluence par heures",
        data : [0,0,0,0,0,0,10,80,180,100,25,62,52,5,85,95,85,25,10,2,0,0,0,0],
        backgroundColor: 'rgb(90,90,185)',
        borderColor: '#383838',
        color :'#ffffff',
        borderWidth: 1,
        borderRadius:10,
        categoryPercentage : 1,
        barPercentage : 1,
      }]
    },
    options: {
      scales: {
        y: {
          beginAtZero: true,
          ticks: { color: 'rgb(184,184,189)'},
          grid : {
            color : 'rgba(143,143,143,0.56)'
          }
        },
        x: {
          ticks: { color: 'rgb(184,184,189)'},
          grid : {
            color : 'rgb(143,143,143,0.56)'
          }
        },
      },
    },
  })
}
build_chart_player_online()
