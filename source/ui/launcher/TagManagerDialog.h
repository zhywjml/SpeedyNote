#ifndef TAGMANAGERDIALOG_H
#define TAGMANAGERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

/**
 * @brief Modal dialog for managing tags on a notebook.
 *
 * TagManagerDialog provides a touchscreen-friendly interface for:
 * - Viewing all available tags in the library
 * - Creating new tags
 * - Assigning/unassigning tags to the current notebook
 * - Deleting unused tags
 *
 * Tags are stored in each notebook's document.json, but this dialog
 * collects all unique tags across all notebooks for management.
 *
 * Step 1: Tag feature - Part of tag management UI.
 */
class TagManagerDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Construct a tag manager dialog.
     * @param bundlePath Path to the notebook bundle (empty for library-wide view).
     * @param parent Parent widget.
     */
    explicit TagManagerDialog(const QString& bundlePath, QWidget* parent = nullptr);

    /**
     * @brief Get the list of tags assigned to the notebook after dialog closes.
     * @return List of tag names.
     */
    QStringList selectedTags() const { return m_selectedTags; }

    /**
     * @brief Set dark mode for theming.
     * @param dark True for dark mode.
     */
    void setDarkMode(bool dark);

    /**
     * @brief Static convenience method to show the dialog and get tags.
     * @param bundlePath Path to the notebook bundle.
     * @param parent Parent widget.
     * @param currentTags Currently assigned tags.
     * @return Selected tags, or empty list if cancelled.
     */
    static QStringList getTags(const QString& bundlePath,
                              QWidget* parent,
                              const QStringList& currentTags = QStringList());

private slots:
    void onSearchTextChanged(const QString& text);
    void onTagClicked(QListWidgetItem* item);
    void onAddTagClicked();
    void onDeleteTagClicked();
    void onApplyClicked();
    void onCancelClicked();

private:
    void setupUi();
    void applyTheme();
    void populateTags();
    void filterTags(const QString& text);

    /**
     * @brief Get all unique tags from all notebooks in the library.
     */
    QStringList getAllTagsFromLibrary() const;

    /**
     * @brief Save tags to the notebook's document.json.
     */
    void saveTagsToNotebook(const QStringList& tags);

    // UI components
    QLabel* m_titleLabel = nullptr;
    QPushButton* m_closeButton = nullptr;
    QLineEdit* m_searchInput = nullptr;

    // Tag list showing all available tags
    QListWidget* m_tagList = nullptr;

    // Current tags (selected for the notebook)
    QListWidget* m_selectedTagList = nullptr;

    QPushButton* m_addTagButton = nullptr;
    QPushButton* m_deleteTagButton = nullptr;
    QPushButton* m_applyButton = nullptr;
    QPushButton* m_cancelButton = nullptr;

    // State
    QString m_bundlePath;
    QStringList m_allTags;          // All tags from library
    QStringList m_selectedTags;     // Tags currently assigned to notebook
    bool m_darkMode = false;

    // Layout constants for touch-friendly UI
    static constexpr int ITEM_HEIGHT = 44;
    static constexpr int BUTTON_HEIGHT = 40;
    static constexpr int MARGIN = 16;
    static constexpr int SPACING = 12;
    static constexpr int SEARCH_HEIGHT = 40;
    static constexpr int DIALOG_MIN_WIDTH = 360;
    static constexpr int DIALOG_MIN_HEIGHT = 480;
};

#endif // TAGMANAGERDIALOG_H