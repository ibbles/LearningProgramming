CSS stands for Cascading Style Sheets.
A CSS file contains a set of rules.
Each rule consists of a selector and a list of declarations in the form of property-value pairs.
```css
div.bold-text { /* Selector */
	/* Delclarations: */
	font-weight: 700;
/*  property     value */
}
```

Associate a CSS file with an HTML document with 
# Selectors

- `*`: Everything.
- `div`: Add `div`s.
- `p`: All paragraphs.
-  `.CLASS`: Select all elements with class `CLASS`.
- `#ID`: Select the element that has ID `ID`.

A selector can consists of multiple parts.
The separator used between the parts define how they are combined:
- `,`: `OR`, combining (?) selector.
	- An element is selected if it matches at least one of the selectors.
- (empty string): `AND`, chaining selector.
	- An element is selected if it matches all of the selectors.
- ` ` (space): Descendant combinator.
	- A chain of selectors applied to anscestors.

We can `OR` multiple selectors in a rule by listing them with `,`:
```css
.some-class,
.some-other-class {
	/* Shared declarations */
}

.some-class {
	/* Declarations specific to some-class. */
}

.some-other-class {
	/* Declarations specific to some-other-class. */
}
```

We can `AND` multiple selectors in a rule by concatenating them:
```css
.some-class.some-other-class {
	/* Declarations that apply to elements that have some-class AND some-other-class. */
}
```

An `AND` concatenation can only have _one_ type selector, e.g, `div` or `p`.
Example of an `AND` chain with multiple types of selectors:
```css
div.some-class#some-id {
	/* Declarations that apply to any 'div' that has the class some-class and the ID some-id. */
}
```

We can select nested objects with the descendant combinator.
```css
.ancestor .child {
	/* Declarations that apply to .child if and only if it is a decendant of .ancestor. */
}
```

When multiple selectors are added to the descendant combinator then they apply right-to-left upwards the hierarchy.
```css
.grandparent .parent .child .grandchild
```
To determine whether a element should be selected, we first check if it has the `grandchild` class.
If it does, then we check that it is a descendant of an element with the `child` class.
If it is, then we move focus to that ancestor.
We check if that ancestor is a descendant of an element with the `parent` class.
If it is, then we move focus to that ancestor.
We check if that ancestor is a descendant of an element with the `grandparent` class.
This process will continue for as many ancestors as there are selectors in the descendant combinator.


# Properties

- `color`: Foreground color. Can be given in hex, RGB, or HSL.
	- hex: `#0080ff`
	- RGB: `rgb(0, 128, 255`
	- HSL: `hsl(210, 100% 50%)`
- `background-color`
- `font-family`: Comma-separated list of font names.
	- Font family names uses double-quotes around them.
	- Generic family names do not use any quotes.
	- The browser will use the first font it supports.
	- Therefore, the list should end with a widely supported generic font family.
	- Example: `"Times New Roman", serif`
- `font-size`: The size of the font, including unit.
	- Example: `22px`
- `font-weight`: How bold the font should be.
	- Either a name of a number between 1 and 1000.
	- A common name is `bold`.
- `text-align`: Horizontal alignment of the element.
	- `left`
	- `center`
	- `right`
- `width`/`height`: The dimensions of e.g. an image.
	- Set one of them to `auto` to resize without altering the proportions.
	- For images, the width and height should be specified in both the HTML and the CSS.
	- This is to reserve the correct amount of page real-estate before the image has been loaded.