## Lab 8: Implementing a Flask-based MQTT Data Visualization Application

**Prerequisites**
- Python 3.x installed
- Basic understanding of Python programming
- SQLite database named 'historian_data.db' containing MQTT messages
- Required Python packages: Flask, matplotlib, sqlite3

**Setup Instructions**
1. Navigate to the folder containing your `historian.py` program. This program must access the same database file.
2. Create a folder called `templates` from this location.
3. Enter the project virtual environment, probably with the `source IoT/bin/activate` directive.
4. Install required packages:
```bash
pip install flask matplotlib
```

The packages will enable us to write a visualisation Web application:
- Flask is a specialised package to enable building Web application.
- Matplotlib is a package to produce graphic data plots such as X-Y graphs.

## Implementation Steps

In the SAME FOLDER as the `historian.py` program, create `app.py` as follows:
```bash
nano app.py
```

Add the following code:

**Step 1: Initial Setup and Imports**
```python
from flask import Flask, render_template
import sqlite3
import matplotlib.pyplot as plt
import io
import base64
import matplotlib.dates as mdates
from datetime import datetime
```

We will import
- Flask for all Web features,
- sqlite3 to enable database-reading,
- matplotlib to draw the graphs
- io to allow data storage like in a file but in memory
- base64 to encode the image files for direct output to a webpage
- datetime to handle date-time data

**Step 2: Flask Application Initialization**
Add the Flask app initialization:
```python
app = Flask(__name__)
```

This adds the Flask object that will be able to run the website that we will define here.

**Step 3: Database Access Functions**
Create the function to retrieve topics:
```python
def get_topics():
    conn = sqlite3.connect('historian_data.db') #connect to the SQLite3 DataBase file
    cursor = conn.cursor()  #create a cursor to execute commands and get data back
    SQL = "SELECT DISTINCT topic FROM historian_data" #define the SQL to get the topics list
    cursor.execute(SQL) #run the SQL command
    topics = [row[0] for row in cursor.fetchall()] #returned results are always placed in an array, extract the first item (topic)
    conn.close() #close the connection
    return topics
```

This function will extract the topic data from the historian_data table.
This is done by pulling the name of each topic only once when we select **DISTINCT** topic from the table.

Create the function to retrieve data for each topic:
```python
def get_data_for_topic(topic):
    conn = sqlite3.connect('historian_data.db') #connect to the SQLite3 DataBase file
    cursor = conn.cursor() #create a cursor to execute commands and get data back
    SQL = "SELECT timestamp, message FROM historian_data WHERE topic = ? ORDER BY timestamp" #SQL to get messages and timestamnps for this topic
    #run the SQL with the topic to complete the SQL command
    #(topic,) ensures that the list provides all the extra useless data needed by the execute function while passing only our topic
    cursor.execute(SQL, (topic,))
    #Store all the records and close the "connection" to the file
    data = cursor.fetchall()
    conn.close() #close the connection
    # empty arrays will hold data
    timestamps = []
    values = []
    text_annotations = []
    #for each record
    for timestamp, message in data:
        timestamps.append(datetime.fromisoformat(timestamp))
        try: #if the data is numeric...
            value = float(message)
            values.append(value)
            text_annotations.append(None)
        except ValueError: # If it's text, store it as an annotation
            values.append(None)  # Use None for text points
            text_annotations.append(f"{topic}: {message}")
    return timestamps, values, text_annotations # return the parallel arrays
```

The function does more than just get the data from the database table.
The function places the data into 3 parallel arrays:
- one for the timestamps
- one for the numnerical data
- one for text data (we call them annotations)
The text data is separate because it is hard to place on a graph at any specific location.

**Step 4: Main Route and Plotting Function**
Implement the main route that creates the visualization.
A route is a path in a Web application.
For example, at `https://cstutoring.ca/` you find the main page of my blog but at `https://cstutoring.ca/sitemap/` you find the list of all things on my blog website.
Here, we define the thing that happens when we call this web app without additional path, just like `https://cstutoring.ca/`.

However, since we the web server address is defined by things outside the program, this part is left out.

The main things that this function will do are the following:
- Define the start of an X-Y graphic data plot.
- Get all topics from the DB.
- For each topic, get all data for each topic in the DB and place it on the graphic data plot, along with the label for this data.
- Define the main title for the graph, as well as the axis labels, the legend, and all other formating.
- Create a buffer to save the file in memory and save the graphic plot to the format of a file within this buffer.
- Encode this buffer to base64. This is the format which allows embedding the image directly in the HTML output.
- Call the function that will output the HTML (with the encoded image).

```python
@app.route('/')
def plot_data():
    #we will create an image to represent the data plot
    #This creates a new figure (window or page) for plotting.
    # figsize sets the width, height of the figure in inches. Here we set 12 inches wide by 6 inches tall.
    plt.figure(figsize=(12, 6))
    #Get current axes and assign them to axes
    axes = plt.gca()
    axes.clear()

    #get topics from DB
    topics = get_topics()
    #for each topic
    for i, topic in enumerate(topics):
        #get the data
        timestamps, values, text_annotations = get_data_for_topic(topic)
        if not timestamps:
            continue
        # Create arrays for numeric data points
        num_timestamps = []
        num_values = []
        # Create arrays for text annotation points
        text_timestamps = []
        annotations = []
        # Separate numeric and text data
        for ts, val, text in zip(timestamps, values, text_annotations):
            if val is not None:
                num_timestamps.append(ts)
                num_values.append(val)
            if text is not None:
                text_timestamps.append(ts)
                annotations.append(text)
        # Plot numeric data
        if num_timestamps:
            axes.plot(num_timestamps, num_values, label=topic, color=f'C{i}', linewidth=1, marker='.')
        # Add text annotations
        for ts, text in zip(text_timestamps, annotations):
            axes.annotate(text,
                       xy=(ts, axes.get_ylim()[1]),  # Place at top of chart, above the correct timestamp
                       xytext=(0, 10),  # 10 points above
                       textcoords='offset points',
                       ha='center', #horizontal and vertical alignment properties
                       va='bottom',
                       bbox=dict(boxstyle='round,pad=0.2', fc=f'C{i}', alpha=0.5), #box around the annotation
                       rotation=45)
            plt.axvline(x=ts,ymin=0.1, ymax=0.9, color=f'C{i}') #draw a vertical line denoting where the command in the label happened

    #autoformat x axis to be non-overlaping dates
    plt.gcf().autofmt_xdate()
    #Set the date format
    axes.xaxis.set_major_formatter(mdates.DateFormatter('%Y-%m-%d %H:%M:%S'))
    #Add a grid of dashes lines with 70% opacity (alpha)
    plt.grid(True, linestyle='--', alpha=0.7)
    #place the legend upper left a bit outside the plot
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left')
    #Add a title
    plt.title('Historian Data Visualization')
    #add a label to the x axis
    plt.xlabel('Timestamp')
    #add a label to the y axis
    plt.ylabel('Value')
    # Add 20% margins to make room for annotations
    plt.margins(y=0.2)
    #auto-adjust layout to avoid overlapping
    plt.tight_layout()

    #create a space in memory to store the image
    buf = io.BytesIO()
    #save the image to the buffer memory
    plt.savefig(buf, format='png', bbox_inches='tight')
    #close the current plot to free up resources
    plt.close()

    #rewind the buffer to the beginning (to play it again later)
    buf.seek(0)
    #read the image and encode it to base64 for direct output in the web page
    image = base64.b64encode(buf.getvalue()).decode('utf-8')

    #Render the plot.html template with the image in it for the web browser to see
    return render_template('plot.html', image=image)
```

Yes, guilty, this function is TOO BIG!

**Step 5: Application Entry Point**
Add the main execution block.
As before this will ensure that we are running this file directly in a `python3 app.py` instruction and that the file is not being called from another program such as `python3 otherprogram.py` that would have diferent calls.
```python
#make the app run if this is the file Python3 runs
if __name__ == '__main__':
    app.run(debug=True)
```

OK. The `app.py` program is complete, but it needs one more dependency.

**Step 6: HTML Template**
From the location of the `app.py` program, create a folder called `templates`:
```bash
mkdir templates
```

Go to this new folder:
```bash
cd templates
```

Create the new file `plot.html` in the templates folder:
```bash
nano plot.html
```

in the file, place the following HTML code:
```html
<!DOCTYPE html>
<html>
<head>
    <title>Historian Data Visualization</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 20px;
            background-color: #f0f0f0;
        }
        .container {
            background-color: white;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0,0,0,0.1);
        }
        h1 {
            color: #333;
            text-align: center;
        }
        .plot-container {
            text-align: center;
            margin-top: 20px;
        }
        img {
            max-width: 100%;
            height: auto;
        }
        #last-update {
            text-align: center;
            color: #666;
            margin-top: 10px;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Historian Data Visualization</h1>
        <div class="plot-container">
            <img src="data:image/png;base64,{{ image }}" alt="Historian Data Plot">
        </div>
        <div id="last-update">Last updated: <span id="update-time"></span></div>
    </div>
    <script>
        function updateDateTime() {
            const now = new Date();
            document.getElementById('update-time').textContent = now.toLocaleString();
        }

        function refreshPage() {
            window.location.reload();
            updateDateTime();
        }

        // Update time immediately
        updateDateTime();
        
        // Refresh every 60 seconds
        setInterval(refreshPage, 60000);
    </script>
</body>
</html>
```
This document is simpler than it may look.
The `<!DOCTYPE...>` directive declares that this a html document.
The `<html>` element, closed at the end with `</html>` contains the entire document.

The `<head>` section contains information about the document:
- the `<title>` appears in the header title when opening this webpage.
- the `<style>` section contains styling directives so the webpage is not too ugly.

The `<body>` section contains all the things that will display on the page:
- `<div class="container">` is a block to contain the page and ensure its styling is applied correctly.
- `<h1>Historian Data Visualization</h1>` shows *Historian Data Visualization* as the top-level heading.
- `<div class="plot-container">` defines the area styling for the image inside.
- `<img src="data:image/png;base64,{{ image }}" alt="Historian Data Plot">` will output the image data encoded to base-64 placed where we see `{{ image }}`.
- `<div id="last-update">Last updated: <span id="update-time"></span></div>` is there to accept the time of the last update which will be written by the following JavaScript:
```javascript
    <script>
        function updateDateTime() {
            const now = new Date(); //make a new date
            document.getElementById('update-time').textContent = now.toLocaleString(); //place the new date text in the update-time span above
        }

        function refreshPage() {
            window.location.reload();//force refresh of the page when called
            updateDateTime();//call the function above to display the new time
        }

        // Update time immediately
        updateDateTime(); //upon loading the page, update the time to show it
        
        // Refresh every 60 seconds
        setInterval(refreshPage, 60000); //60 seconds from now, call the refreshPage function
    </script>
```
Each instruction is commented with its meaning.

## Running the Application
1. Ensure your historian_data.db file is in the same directory as app.py
2. Open a terminal in your project directory
3. Run the application:
```bash
python3 app.py
```
This should tell you that the application is accessible from port 5000.
4. Open a web browser and navigate to http://localhost:5000

The application will display a graph showing all MQTT topics and their corresponding values over time, with text annotations for non-numeric messages.

