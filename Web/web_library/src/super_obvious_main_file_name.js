const express = require("express");
const fs = require("fs");
const path = require("path");
const app = express();
const SETUP = require("./setup/setup.js");
app.use(express.json());
app.get("/", (req, res) => {
  res.sendFile("index.html", { root: __dirname });
});

app.post("/book", (req, res) => {
  const book = req.body && req.body.book;
  switch (book) {
    case "book1":
      res.json({
        error: false,
        data: {
          bookName: "The Great Gatsby",
          bookCover: "gatsby.jpg",
          description:
            "The Great Gatsby is a 1925 novel by American writer F. Scott Fitzgerald. Set in the Jazz Age on Long Island, near New York City, the novel depicts first-person narrator Nick Carraway's interactions with mysterious millionaire Jay Gatsby and Gatsby's obsession to reunite with his former lover, Daisy Buchanan.",
        },
      });
      break;
    case "book2":
      res.json({
        error: false,
        data: {
          bookName: "1984",
          bookCover: "1984.jpg",
          description:
            "Nineteen Eighty-Four is a dystopian social science fiction novel and cautionary tale by English writer George Orwell. It was published on 8 June 1949 by Secker & Warburg as Orwell's ninth and final book completed in his lifetime.",
        },
      });
      break;
    case "book3":
      res.json({
        error: false,
        data: {
          bookName: "To Kill a Mockingbird",
          bookCover: "mockingbird.jpg",
          description:
            "To Kill a Mockingbird is a novel by the American author Harper Lee. It was published in 1960 and was instantly successful. In the United States, it is widely read in high schools and middle schools. To Kill a Mockingbird has become a classic of modern American literature, winning the Pulitzer Prize.",
        },
      });
      break;
    default:
      res.json({ error: true, data: { message: "Book not found." } });
      break;
  }
});

app.get("/covers", (req, res) => {
  let img = req.query && req.query.img;

  if (!img) res.send("");
  // No hacking allowed
  img = img.replaceAll("../", "");

  res.sendFile(path.join("/static/covers", img), { root: __dirname });
});

SETUP();

app.listen(8080, () => {
  console.log("Listening on port 8080...");
});
