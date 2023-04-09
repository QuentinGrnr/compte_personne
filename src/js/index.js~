const graphChoices = document.querySelectorAll('.graph_choice');
const graphs = document.querySelectorAll('.graph');

graphChoices.forEach(choice => {
    choice.addEventListener('click', () => {
        const graphId = choice.dataset.graphId;
        graphs.forEach(graph => {
            if (graph.id === graphId) {
                graph.style.display = 'block';
            } else {
                graph.style.display = 'none';
            }
        });
    });
});