// Replace this with your actual API Gateway endpoint
const apiUrl = 'https://chtm78bc6b.execute-api.eu-north-1.amazonaws.com/prod';

async function fetchParkingData() {
    try {
        let response = await fetch(apiUrl);
        let data = await response.json();
        updateParkingSpots(data);
    } catch (error) {
        console.error('Error fetching parking data:', error);
    }
}

function updateParkingSpots(data) {
    const parkingSpotsDiv = document.getElementById('parking-spots');
    parkingSpotsDiv.innerHTML = ''; // Clear previous data

    data.forEach(spot => {
        const spotDiv = document.createElement('div');
        spotDiv.classList.add('parking-spot');
        spotDiv.classList.add(spot.status); // 'occupied' or 'available'

        spotDiv.innerHTML = `
            <h3>Space ID: ${spot.spaceID}</h3>
            <p>Status: ${spot.status}</p>
            <p>Time: ${new Date(spot.time).toLocaleString()}</p>
        `;

        parkingSpotsDiv.appendChild(spotDiv);
    });
}

// Fetch data when the page loads
window.onload = fetchParkingData;
