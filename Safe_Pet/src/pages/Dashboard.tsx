import React from 'react';
import '../styles/Dashboard.css';

export default function Dashboard(){

	return(

		<div className="corpo">
			<aside className="aside">
				<section className="option_button">
					<p>aside button</p>
  				</section>

				<section className="option_button">
					<p>aside button</p>
  				</section>
	
  			</aside>


			<section className="main">

				<header className="header">
					<p>header</p>
  				</header>

				<section className="content">
					<p>main</p>
  				</section>
  			</section>
		</div>

    			
);
}