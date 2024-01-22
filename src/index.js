// Firebase Imports
import { initializeApp } from 'https://www.gstatic.com/firebasejs/10.7.1/firebase-app.js';
import { getDatabase, ref, set, onValue } from 'https://www.gstatic.com/firebasejs/10.7.1/firebase-database.js';

// Firebase Configuration
const firebaseConfig = {
  apiKey: "AIzaSyCH7NojcKtJIG98LKan1WxfFsDIn1bNe9A",
  authDomain: "smart-home-d65f6.firebaseapp.com",
  databaseURL: "https://smart-home-d65f6-default-rtdb.firebaseio.com",
  projectId: "smart-home-d65f6",
  storageBucket: "smart-home-d65f6.appspot.com",
  messagingSenderId: "658367806697",
  appId: "1:658367806697:web:c866103d0ba4e2b2658478"
};
const app = initializeApp(firebaseConfig);
const database = getDatabase(app);

// UI Elements
const lightCheckbox = document.getElementById('light');
const luminosityRange = document.getElementById('luminosity');
const lockCheckbox = document.getElementById('lock');
const watersprayCheckbox = document.getElementById('waterspray');
const windowCheckbox = document.getElementById('window');
const gasReading = document.getElementById('gas');
const cameraObject = document.getElementById('cameraObject')

// UI Control Elements
const lightUI = document.getElementById('lightUI');
const luminosityUI = document.getElementById('luminosityUI');
const lockUI = document.getElementById('lockUI');
const watersprayUI = document.getElementById('watersprayUI');
const windowUI = document.getElementById('windowUI');

// Firebase References
const lightref = ref(database, 'data/light');
const windowref = ref(database, 'data/window');
const luminosityref = ref(database, 'data/luminosity');
const gasref = ref(database, 'data/gas');
const watersprayref = ref(database, 'data/waterspray');
const lockref = ref(database, 'data/lock');
const cameraObjref = ref(database, 'data/cameraObject')

// Audio for Alerts
const audio = new Audio('./assets/fire_alarm.mp3');

// Loading Screen Handling
setTimeout(() => {
  document.getElementById('loading').style.display = 'none';
}, 2000);

// Event Handlers for UI Control Elements
lightUI.onclick = () => set(lightref, !lightCheckbox.checked);
luminosityRange.oninput = () => set(luminosityref, luminosityRange.value);
windowUI.onclick = () => set(windowref, !windowCheckbox.checked);
lockUI.onclick = toggleDoorLock; // Uses the function defined below
watersprayUI.onclick = () => set(watersprayref, !watersprayCheckbox.checked);

// Real-time Data Listeners
onValue(lightref, snapshot => lightCheckbox.checked = snapshot.val());
onValue(lockref, snapshot => lockCheckbox.checked = snapshot.val());
onValue(windowref, snapshot => windowCheckbox.checked = snapshot.val());
onValue(watersprayref, snapshot => watersprayCheckbox.checked = snapshot.val());
onValue(luminosityref, snapshot => luminosityRange.value = snapshot.val());

//Object dectection
let defaultObj = 0;
onValue(cameraObjref,(snapshot)=>{
  const data = snapshot.val();
  cameraObject.innerHTML = data;

  //Alert when detect an object
  if (defaultObj == 0 && data === 1){
    alert("An Object has been Detected");
  }
  defaultObj=data;
});


// Gas Leak Detection
let previousGasReading = 0;
onValue(gasref, (snapshot) => {
  const data = snapshot.val();
  gasReading.innerHTML = data;
  
  // Gas Leak Alert
  if (previousGasReading === 0 && data === 1) {
    alert("Warning: Gas leak detected! This could lead to a house fire.");
    audio.play();
  }

  previousGasReading = data;
});

// Numeric Password for Door Lock
const doorCode = 1234; 

// Modal Handling
const modal = document.getElementById("codeModal");
const span = document.getElementsByClassName("close")[0];

span.onclick = closeModal;
window.onclick = event => {
  if (event.target == modal) {
    closeModal();
  }
};

// document.getElementById('submitCode').addEventListener('click', submitCode);
document.getElementById('passwordForm').addEventListener('submit', function(event) {
  event.preventDefault(); // Prevent the default form submission
  submitCode();
});
// Function Definitions

function closeModal() {
  modal.style.display = "none";
  resetLockToggle();
}

function submitCode() {
  const enteredCode = document.getElementById('codeInput').value;
  if (parseInt(enteredCode, 10) === doorCode) {
    set(lockref, true);
    modal.style.display = "none";
  } else {
    alert("Incorrect code. Unable to change the door lock state.");
    resetLockToggle();
  }
}

function resetLockToggle() {
  lockCheckbox.checked = false;
}

function toggleDoorLock() {
  if (!lockCheckbox.checked) {
    document.getElementById('codeInput').value = ''; 
    modal.style.display = "block";
  } else {
    set(lockref, !lockCheckbox.checked);
  }
}

