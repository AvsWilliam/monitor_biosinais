import App from './App.tsx';
import './styles/Dashboard.css';
import React from 'react';
import { StrictMode } from "react";
import { createRoot } from "react-dom/client";



const rootElement = document.getElementById("root");
const root = createRoot(rootElement);

	root.render(
		<StrictMode>
			<App />
		</StrictMode>
);



