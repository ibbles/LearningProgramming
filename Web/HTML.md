# File
A HTML file is a textual code description of a document.
The filename suffix for an HTML file is `.html`.
The main page of a site should be called `index.html`.

# Elements And Tags

The document is made up of elements.
An element has a type.
The type is identified by a tag.
Some elements have content, often text or other elements.
Tags that mark elements that do not have any content are called void tags.
Tag names are enclosed by `<` and `>` in the HTML code.
Non-void tags mark the end of their content with the tag name enclosed in `</` and `>`.
The `<` `>` is called the opening tag and the `</` `>` is called the closing tag.

Let's consider a paragraph element.
The name of the tag for a paragraph is `p`.
This means that the start tag is `<p>` and the end tag is `</p>`
Here is an example paragraph:
```html
<p>This is a paragraph.</p>
```

An example of a void tag is `img`.
The `img` tag creates an image element.
Since the image is it's own thing, it is not a property of some text, it doesn't not need any content and thus no closing `</img>` tag.
```html
<img src="my_image.jpg">
```

As seen in the `img` example, a tag can have attributes which add meta-data to the element.

When tags contain other tags we often indent the inner tag:
```html
<html>
	<head>
		<title>My Title</title>
	</head>
	<body>
		<h1>My Heading</h1>
		<p>My text.</p>
	</body>
</html>
```


# Header

A HTML document has a header.
The header includes the DOCTYPE.
The DOCTYPE include the version of HTML the document uses.
The current / default version is 5, so not specifying a version will mean 5.
```html
<!DOCTYPE html>
```


# Root Element

All HTML documents have a root element with the `html` tag.
The `html` element encompasses the entire document, all other elements are content of the `html` element.
```html
<html lang="en">
</html>
```

`lang` is an attribute of the `html` tag.
It identifies the language the document is written in.


# Head Element

The `head` element provide additional meta-information about the document.
(
I don't know what decides which meta-information should go in the `html` element and which should go in the `head` element.
)
The `head` element should contain a `meta` element and a  `title` element.
The `meta` element contains meta-information about the document, such as the character encoding used.
We now have three places for meta-information:
- The `html` element.
- The `head` element.
- The `meta` element.

Example:
```html
<!DOCTYPE html>
<html lang="us">
	<head>
		<meta charset="UTF-8">
		<title>My Title</title>
	</head>
</html>
```


# Body Element

The `body` element contains the contents of the document.
It is nested within the `html` element, immediately following the `head` element.

```html
<!DOCTYPE html>
<html lang="us">
	<head>
		<meta charset="UTF-8">
		<title>My Title</title>
	</head>
	<body>
		This is the document contents.
	</body>
</html>
```


# Text Elements

Text elements are elements that denote different types of text elements.
- `p`: paragraph
	- A paragraph of text.
	- Paragraphs are separated by some vertical white space.
- `h#` where `#` is `[1-6]`: heading
	- 1 is the largest, top-level heading.
	- 6 is the smallest, leaf-level heading.
- `strong`: mark some part of the text important.
	- Will be rendered as bold, but is semantically more than just bold.
	- Can be nested within other text elements.
- `b`: visual bold
	- Change the text rendering without altering any semantics.
- `em`: emphasis
	- Will be rendered as italic, but is semantically more than just italic.
	- Can be nested within other text elements.
- `i`: visual italic
	- Change the text rendering without altering any semantics.


# References

- 1: [_The Odin Project_ > _Working with Text_ @ theodinproject.com](https://www.theodinproject.com/lessons/foundations-working-with-text)
